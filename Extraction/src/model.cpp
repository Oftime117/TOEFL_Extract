#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>

#include "model.h"
#include "essay.h"

using namespace std;

// Methodes hors classe

float avg(float tab[], size_t taille){
    float somme = 0.f;
    for(size_t i=0; i<taille; i++){
        somme += tab[i];
    }
    return somme/(float)taille;
}

int getIndiceMax(const int tab[], size_t taille){
    int max = 0;
    for(size_t i=1; i<taille; i++){
        if(tab[i]>tab[max]) max = i;
    }
    return max;
}

int getLangueIndice(const vector<string> langues, string langue){
    for(size_t i=0; i<langues.size(); i++){
        if(langues[i] == langue) return i;
    }
    return -1;
}

// Methodes de Classse Model

Model::Model(string corpusPath, string featuresOut, string langMatrixOut)
    : trainPath(corpusPath), featuresPath(featuresOut), langMatrixPath(langMatrixOut),
    m_languages(), m_featuresDico(), m_langMatrix(), m_corpusList()
{
    initModel();
}

Model::Model(string featuresIn, string langMatrixIn):featuresPath(featuresIn), langMatrixPath(langMatrixIn), m_languages(), m_featuresDico(), m_langMatrix(){
    loadFeaturesDictionnary();
    loadLangDictionnary();
}

Model::Model(vector<Essay> &corpusList):m_corpusList(corpusList){

    cout<<"m_corpusList taille ==== "<<m_corpusList.size()<<endl;

    // Initialisation de la matrice avec des 0
    m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));
}

Model::~Model()
{
    //dtor
}

//private
void Model::train(vector<Essay> &corpus){
    size_t i=0, x=10;
    float forceCorrection = 1;
    int nbErrors = 1;

    while(i<x && nbErrors>0){ // Condition d'arr�t : x = nombre de tours max OU zero erreur au tour pr�c�dent
        nbErrors = 0;
        forceCorrection = (float)1/(float)(i+1);

        //evaluer chaque essay de train
        // shuffle trainCorpus
        random_shuffle(corpus.begin(), corpus.end());
        for(size_t j=0; j<corpus.size(); j++){
            if( ! evaluer(corpus[j], forceCorrection)){
                nbErrors++;
            }
        }

        float err = ((float)nbErrors / (float)corpus.size()) * (float)100;
        cout << "Step " << i << " - Erreurs : " << nbErrors << " / " << corpus.size() << " (" << err << " %)\n";

        i++;
    }
}

//public
void Model::trainAll(){
    train(m_corpusList);
}

//public
float Model::trainByDiv(size_t nbDiv){
    vector<Essay> trainCorpus;
    vector<Essay> testCorpus;

    float err[nbDiv];
    for(size_t numTrain=0; numTrain<nbDiv; numTrain++){

        cout<<"=== Train "<<numTrain+1<<" / "<<nbDiv<<" ===\n";
        // reset avec des 0 partout
        m_langMatrix.clear();
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));

        trainCorpus.clear();
        testCorpus.clear();

        //prendre un corpus de textes servant a l'�valuation
        for(size_t i=0; i<m_corpusList.size(); i++){

            if(i%nbDiv == numTrain){
                testCorpus.push_back(m_corpusList[i]);
            }
            else{
                trainCorpus.push_back(m_corpusList[i]);
            }
        }

        //entrainer le trainCorpus
        train(trainCorpus);
        //estimer le taux d'erreur sur ce corpus
        int nbErrors = 0;
        // shuffle testCorpus
        random_shuffle(testCorpus.begin(), testCorpus.end());
        for(size_t j=0; j<testCorpus.size(); j++){
            if( ! evaluer(testCorpus[j])){
                nbErrors++;
            }
        }
        err[numTrain] = ((float)nbErrors / (float)testCorpus.size()) * (float)100;
        //Afficher pourcentage d'erreurs
        cout << "Erreurs : " << nbErrors << " / " << testCorpus.size() << " (" << err[numTrain] << " %)\n\n";
    }

    // reset avec des 0 partout
    m_langMatrix.clear();
    m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));

    float errMoy = avg(err, nbDiv);
    cout << "Moyenne erreurs = " << errMoy << " %\n\n" ;


    // Sauvegarder les r�sultats de chaque train dans un fichier
    size_t pos = featuresPath.find(".");
    string path = featuresPath.substr(0, pos) + "_train.txt";
    cout << path << endl;
    cout << ">> Sauvegarde des resultats de " << nbDiv << " entrainements\n\n";

    ofstream cFile(path, ios::out | ios::app);
    if(!cFile){
        //throw une exception
        return -1;
    }
    for(size_t i=0; i<nbDiv; i++){
        cFile << "Train " << i << " = " << err[i] << " %\n";
    }
    cFile << "Moyenne = " << errMoy << endl << endl;
    cFile.close();

    return errMoy;
}

//retourne l'indice de la langue
bool Model::evaluer(Essay &e, float forceCorrection){
    set<int> found;

    vector<string>* listP; listP = e.getWordsListP();
    for(size_t i=0; i<listP->size(); i++){
        string word = (*listP)[i];
        int ind = m_featuresDico["NB_W_" + word];
        if(ind >= 0) found.emplace(ind);
    }

    //calculer le score pour chaque langue
    int score[m_languages.size()];
    for(size_t j=0; j<m_languages.size(); j++){
        score[j] = 0;
    }
    for(int i : found){
        for(size_t j=0; j<m_languages.size(); j++){
            score[j] += m_langMatrix[i][j];
        }
    }

    int lang = getIndiceMax(score, m_languages.size());
    if(m_languages[lang] != e.getLang()){
        //avoir la numero de la langue du texte
        int numLangText = getLangueIndice(m_languages, e.getLang());
        if(forceCorrection > 0){
            corrigerMatrice(forceCorrection, lang, numLangText, found);
        }
        return false;
    }
    return true;
}

void Model::corrigerMatrice(float forceCorrection, size_t langMoins, size_t langPlus, set<int> &foundFeatures){
    for(int ff : foundFeatures){
        m_langMatrix[ff][langMoins] -= forceCorrection;
        m_langMatrix[ff][langPlus] += forceCorrection;
    }
}

void Model::save(){
    saveFeaturesDictionnary();
    saveLangMatrix();
}

void Model::setOutFiles(string featuresOut, string langMatrixOut){
    featuresPath=featuresOut;
    langMatrixPath=langMatrixOut;
}

//private

void Model::initModel()
{
    ifstream corpusFile(trainPath, ios::in);
    if(!corpusFile)
    {
       cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    else
    {
        string line;
        cout << trainPath << endl;
        cout << ">> Lecture du fichier" << endl << endl;

        // Lecture du corpus et enregistrement des caract�ristiques (juste les mots for now))
        set<string> langSet;
        while (getline(corpusFile, line))
        {
            Essay e(line, m_featuresDico, langSet);
            m_corpusList.push_back(e);
        }
        corpusFile.close();
        langSetToVector(langSet);

        // Initialisation de la matrice avec des 0
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));
    }
    corpusFile.close();
}

void Model::saveFeaturesDictionnary()
{
    cout << featuresPath << endl;
    cout << ">> Sauvegarde de " << m_featuresDico.size() << " caracteristiques\n\n";

    ofstream cFile(featuresPath, ios::out);

    if(!cFile)
    {
        //throw une exception
        return;
    }
    cFile << m_featuresDico.size() << endl;
    for(auto &a: m_featuresDico)
    {
        //ecrire chaque mot unique sous la forme d'une caract�ristique
        cFile << a.first << endl;
    }
    cFile.close();
}

void Model::saveLangMatrix()
{
    cout << langMatrixPath << endl;
    cout << ">> Sauvegarde de la matrice des langues [ " << m_featuresDico.size() << " * " << m_languages.size() << " ]\n\n";

    ofstream mFile(langMatrixPath, ios::out);

    if(!mFile)
    {
        //throw une exception
        return;
    }

    mFile << m_languages.size() << "\t" << m_langMatrix.size() << endl;

    // Ajout des headers des langues
    for(string s : m_languages)
    {
        mFile << s;
        if(s != m_languages.back())
            mFile << "\t";
    }
    mFile << endl;

    for(size_t i = 0; i < m_langMatrix.size(); ++i)
    {
        for(size_t j = 0; j < m_langMatrix[0].size(); ++j)
        {
            mFile << m_langMatrix[i][j];
            if(j < m_langMatrix[i].size()-1)
                mFile << "\t";
        }
        if(i < m_langMatrix.size() - 1) mFile << endl;
    }
    mFile.close();
}

void Model::loadFeaturesDictionnary()
{
    cout << ">> Extraction des caracteristiques\n";

    ifstream cFile(featuresPath, ios::in);

    if(!cFile)
    {
        //throw une exception
        return;
    }
    size_t length;
    cFile >> length;

    map<string, int> featuresDico;
    size_t i = 0;
    string feature;
    while(cFile >> feature)
    {
        featuresDico.emplace(feature, i);
        i++;
    }
    cFile.close();

    cout << "(" << length << ") - (" << featuresDico.size() << ")\n\n";
    m_featuresDico = featuresDico;
}

void Model::loadLangDictionnary(){
    cout << ">> Extraction du modele\n";
    ifstream mFile(langMatrixPath, ios::in);

    if(!mFile){
        //throw une exception
        return;
    }
    size_t nbLangues, nbLignes;
    mFile >> nbLangues >> nbLignes;

    set<string> langSet;
    string lang;
    // Extraction de la liste des langues
    size_t l=0;
    for(l=0; l<nbLangues; l++){
        mFile >> lang;
        langSet.emplace(lang);
    }
    langSetToVector(langSet);
    cout << "(" << nbLangues << " langues) * (" << nbLignes << " lignes)\n\n";

    string ligne, score;
    vector<vector<float>> langMatrix(nbLignes);
    for(size_t i=0; i<nbLignes; i++){
        for(l=0; l<nbLangues; l++){
            mFile >> score;
            langMatrix[i].push_back(atof(score.c_str()));
        }
    }
    mFile.close();

    m_langMatrix = langMatrix;
}

void Model::langSetToVector(set<string> langSet){
    //transformer le set en un vector (plus facile a parcourir)
    m_languages.clear();
    for(string s : langSet){
        m_languages.push_back(s);
    }
}
