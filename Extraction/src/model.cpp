#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <random>
#include <chrono>

#include "tools.h"
#include "model.h"
#include "essay.h"
#include "corpus.h"

using namespace std;

// Constantes
const bool Model::printTrainConfusionMatrix = true;


Model::Model(string corpusPath, string featuresOut, string langMatrixOut)
    : m_trainPath(corpusPath), m_featuresPath(featuresOut), m_langMatrixPath(langMatrixOut),
    m_languages(), m_featuresDico(), m_langMatrix(), m_confusionMatrix(), m_corpusList()
{
    initModel();
}

/* pourquoi trainPath et corpusList ne sont pas initialisé?
 * Amirali
 */
Model::Model(string featuresIn, string langMatrixIn):m_featuresPath(featuresIn),
m_langMatrixPath(langMatrixIn), m_languages(), m_featuresDico(), m_langMatrix(), m_confusionMatrix()
{
    loadFeaturesDictionnary();
    loadLangDictionnary();
}

/*** Méthodes publiques ***/
/* Cette fonction est encore utile?
 * Amirali
 */
void Model::trainAll()
{
    //train(m_corpusList);
}

float Model::trainByDiv3(const size_t& nbDiv)
{
    float err[nbDiv];
    vector<future<float> > futures;
    vector<Corpus> corpusTab;
    /*** Lancement de l'entrainement en cross validation ***/
    cout << "Dico size before MT: " << m_featuresDico.size() << endl;
    for(size_t numTrain=0; numTrain<nbDiv; ++numTrain)
    {
        cout<<"=== Ajout Train "<<numTrain+1<<" / "<<nbDiv<<" ===\n";

        corpusTab.push_back(Corpus(m_languages, m_featuresDico, m_corpusList, nbDiv, numTrain));
    }
    for(size_t numTrain=0; numTrain<nbDiv; ++numTrain)
    {
        cout<<"=== Train MT "<<numTrain+1<<" / "<<nbDiv<<" ===\n";
        try
        {
            future<float> fut = async(launch::async, &Corpus::train, &corpusTab[numTrain], false);
            futures.push_back(move(fut));
        }
        catch(const exception& e)
        {
            cout << "Erreur lors de l'ajout du future: " << e.what();
        }
        catch(...)
        {
            cout << "Exception inconnu\n";
        }
    }
    size_t i = 0;
    try
    {
        /* On inverse pour avoir les résultats dans le bon ordre
         * Pas nécessaire je pense
         * Amirali
         */
        //reverse(futures.begin(), futures.end());
        while(!futures.empty())
        {
            future<float> fut = move(futures.back());
            futures.pop_back();
            /** extraire le taux d'erreur **/
            err[i] = fut.get();
            i++;
        }
        for(i=0; i<nbDiv; i++)
        {
            /** extraire la matrice de confusion **/
            Tools::sumMatrix(m_confusionMatrix, corpusTab[i].getConfusionMatrix());
            if(printTrainConfusionMatrix)
            {
                stringstream s; s << "data/confusion_"<<i<<".txt";
                corpusTab[i].printConfusionMatrix(s.str());
            }
        }

        float errMoy = Tools::floatArrayAVG(err, nbDiv);
        cout << "Moyenne erreurs = " << errMoy << " %\n\n" ;

        /*** Sauvegarde des résultats de chaque train dans un fichier ***/
        string path = "data/history.txt";
        cout << path << endl;
        cout << ">> Sauvegarde des resultats de " << nbDiv << " entrainement" << (nbDiv>1?"s":"") << "\n\n";

        ofstream cFile(path, ios::out | ios::app);
        if(!cFile)
        {
            //throw une exception
            return -1;
        }
        for(size_t i=0; i<nbDiv; i++)
        {
            cFile << "Train " << i << " = " << err[i] << " %\n";
        }
        cFile << "Moyenne = " << errMoy << endl;
        cFile.close();
    }
    catch(const system_error& e)
    {
        cout << "System error: " << e.code().message() << endl;
    }
    catch(const exception& e)
    {
        cout << "Exception: " << e.what() << endl;
    }
    catch(...)
    {
        cout << "Exception inconnu\n";
    }

    return 0.0f;
}

float Model::trainByDiv2(const size_t& nbDiv)
{
    float err[nbDiv];
    /*** Lancement de l'entrainement en cross validation ***/
    for(size_t numTrain=0; numTrain<nbDiv; ++numTrain)
    {

        cout<<"=== Train "<<numTrain+1<<" / "<<nbDiv<<" ===\n";

        Corpus trainCorpus(m_languages, m_featuresDico, m_corpusList, nbDiv, numTrain);

        err[numTrain] = trainCorpus.train();

        /** extraire la matrice de confusion **/
        Tools::sumMatrix(m_confusionMatrix, trainCorpus.getConfusionMatrix());
        if(printTrainConfusionMatrix)
        {
            stringstream s; s << "data/confusion_"<<numTrain<<".txt";
            trainCorpus.printConfusionMatrix(s.str());
        }
    }

    float errMoy = Tools::floatArrayAVG(err, nbDiv);
    cout << "Moyenne erreurs = " << errMoy << " %\n\n" ;

    /*** Sauvegarde des résultats de chaque train dans un fichier ***/
    string path = "data/history.txt";
    cout << path << endl;
    cout << ">> Sauvegarde des resultats de " << nbDiv << " entrainements\n\n";

    ofstream cFile(path, ios::out | ios::app);
    if(!cFile)
    {
        //throw une exception
        return -1;
    }
    for(size_t i=0; i<nbDiv; i++)
    {
        cFile << "Train " << i << " = " << err[i] << " %\n";
    }
    cFile << "Moyenne = " << errMoy << endl;
    cFile.close();

    return errMoy;
}

void Model::save()
{
    try
    {
        saveFeaturesDictionnary();
        saveLangMatrix();
    } catch(const runtime_error& e)
    {
        cerr << e.what() << endl;
    }
}

void Model::setOutFiles(string featuresOut, string langMatrixOut)
{
    m_featuresPath=featuresOut;
    m_langMatrixPath=langMatrixOut;
}

void Model::resetConfusionMatrix() {
    /** initialisation de la matrice de confusion a 0 partout */
    m_confusionMatrix.clear();
    m_confusionMatrix.resize(m_languages.size(), vector<int>(m_languages.size()));
}

void Model::printConfusionMatrix(const string& path) const{
    ofstream mFile(path, ios::out);
    if(!mFile)
    {
       cerr << "Impossible d'ouvrir le fichier " << path << endl;
    }
    else
    {
        size_t taille = m_confusionMatrix.size();
        mFile << "\t";
        for(string lang : m_languages){
            mFile << lang << "\t";
        }
        for(size_t i=0; i<taille; i++){
            mFile << endl << m_languages[i] << "\t";;
            for(size_t j=0; j<taille; j++){
                mFile << m_confusionMatrix[i][j] << "\t";
            }
        }
        mFile.close();
    }
}

/*** Méthodes Privées ***/
void Model::initModel()
{
    /*** lancement du script (wapiti) ***/
    //on suppose que les fichiers sont déjà créés
    /*** récupération des résultats wapiti ***/
    ifstream labeledCorpusFile("script/trainTagLine.txt", ios::in);
    ifstream labeledOcc1CorpusFile("script/occurence1TagLine.txt", ios::in);
    ifstream labeledOcc2CorpusFile("script/occurence2TagLine.txt", ios::in);
    //ifstream labeledOcc3CorpusFile("script/occurence3TagLine.txt", ios::in);

    /*** Mots et occurences ***/
    ifstream wordOcc1CorpusFile("script/occurence1MotLineLower.txt", ios::in);
    ifstream wordOcc2CorpusFile("script/occurence2MotLineLower.txt", ios::in);
    //ifstream wordOcc3CorpusFile("script/occurence3MotLineLower.txt", ios::in);

    ifstream corpusFile(m_trainPath, ios::in);
    if(!corpusFile || !labeledCorpusFile || !labeledOcc1CorpusFile || !labeledOcc2CorpusFile
        /*|| !labeledOcc3CorpusFile*/ || !wordOcc1CorpusFile || !wordOcc2CorpusFile /*|| !wordOcc3CorpusFile*/)
    {
       cerr << "Impossible d'ouvrir un des fichiers !" << endl;
       throw -1;
    }
    else
    {
        cout << m_trainPath << endl;
        cout << ">> Lecture du fichier et initialisation des features" << endl << endl;

        /*** Lecture du corpus et enregistrement des caractéristiques ***/
        set<string> langSet;
        string line, labeledLine, labeledOcc1Line, labeledOcc2Line, labeledOcc3Line;
        string wordOcc1Line, wordOcc2Line, wordOcc3Line;
        /* Attention ici mettre des && entre les getlines n'est pas correct ! On sortirait du while
         * dès que l'on aurait fini de lire un des fichiers
         * Peut être mieux de lire les fichiers les uns après les autres?
         * Amirali
         *
         * TODO: Check j'ai trouvé que good() devrait faire ce que je veux mais du coup je dois faire des cas ensuite
         * Si tu as une meilleure idée, n'hesite pas
         */
//         cout << "coucou" <<endl;
        bool b0 = getline(corpusFile, line);
        bool b1 = getline(labeledCorpusFile, labeledLine);
        bool b2 = getline(labeledOcc1CorpusFile, labeledOcc1Line);
        bool b3 = getline(labeledOcc2CorpusFile, labeledOcc2Line);
        //bool b4 = getline(labeledOcc3CorpusFile, labeledOcc3Line);
        bool b5 = getline(wordOcc1CorpusFile, wordOcc1Line);
        bool b6 = getline(wordOcc2CorpusFile, wordOcc2Line);
        //bool b7 = getline(wordOcc3CorpusFile, wordOcc3Line);
        while (b0 && b1 && b2 && b3/* && b4*/ && b5 && b6/* && b7*/)
        {
            try
            {
                //Essay e(line, m_featuresDico, langSet);
//cout << i << " ";
//cout << "1" << endl;
                Essay e(line, labeledLine, labeledOcc1Line, labeledOcc2Line, labeledOcc3Line, wordOcc1Line, wordOcc2Line, wordOcc3Line, m_featuresDico, langSet);
//cout << "2" << endl;
                m_corpusList.push_back(std::move(e));
//cout << "3" << endl;
            }catch(const invalid_argument& e)
            {
                cerr << e.what() << endl;
            }

            b0 = getline(corpusFile, line);
            b1 = getline(labeledCorpusFile, labeledLine);
            b2 = getline(labeledOcc1CorpusFile, labeledOcc1Line);
            b3 = getline(labeledOcc2CorpusFile, labeledOcc2Line);
            //b4 = getline(labeledOcc3CorpusFile, labeledOcc3Line);
            b5 = getline(wordOcc1CorpusFile, wordOcc1Line);
            b6 = getline(wordOcc2CorpusFile, wordOcc2Line);
            //b7 = getline(wordOcc3CorpusFile, wordOcc3Line);
        }
        corpusFile.close();
        m_languages = Tools::setToVector(langSet);


        cout << "featuresDico size: " << m_featuresDico.size() << endl;


        /*** Ajout des caractéristiques personnalisées ***/
        addFeaturePerso("AVG_WORD_CORPUS");
        addFeaturePerso("AVG_LETTER_CORPUS");
        addFeaturePerso("AVG_S_WORD");
        addFeaturePerso("AVG_WEND_ING");
        addFeaturePerso("AVG_FIRSTCAPS");
        addFeaturePerso("AVG_I_CAPS");
        addFeaturePerso("AVG_I_LOW");
        addFeaturePerso("AVG_PRONOMS");
        addFeaturePerso("AVG_THE");
        addFeaturePerso("AVG_S_SENTENCE");
        addFeaturePerso("AVG_POINT");
        addFeaturePerso("AVG_COMMA");
		addFeaturePerso("WAP_NN");
		addFeaturePerso("WAP_IN");
		addFeaturePerso("WAP_DT");
		addFeaturePerso("WAP_NNS");
		addFeaturePerso("WAP_JJ");
		addFeaturePerso("WAP_VB");
		addFeaturePerso("WAP_PRP");
		addFeaturePerso("WAP_RB");
		addFeaturePerso("WAP_VBP");
		addFeaturePerso("WAP_TO");
		addFeaturePerso("WAP_CC");
		addFeaturePerso("WAP_VBZ");
		addFeaturePerso("WAP_MD");
		addFeaturePerso("WAP_VBG");
		addFeaturePerso("WAP_PRP$");
		addFeaturePerso("WAP_VBN");
		addFeaturePerso("WAP_VBD");
		addFeaturePerso("WAP_JJR");
		addFeaturePerso("WAP_NNP");
		addFeaturePerso("WAP_WRB");
		addFeaturePerso("WAP_CD");
		addFeaturePerso("WAP_WDT");
		addFeaturePerso("WAP_WP");
		addFeaturePerso("WAP_RBR");
/*
		addFeaturePerso("WAP_DT_NN");
		addFeaturePerso("WAP_IN_DT");
		addFeaturePerso("WAP_NN_IN");
		addFeaturePerso("WAP_TO_VB");
		addFeaturePerso("WAP_JJ_NN");
		addFeaturePerso("WAP_JJ_NNS");
		addFeaturePerso("WAP_PRP_VBP");
		addFeaturePerso("WAP_DT_JJ");
		addFeaturePerso("WAP_IN_NN");
		addFeaturePerso("WAP_MD_VB");
		addFeaturePerso("WAP_IN_PRP");
		addFeaturePerso("WAP_NNS_IN");
		addFeaturePerso("WAP_VB_DT");
		addFeaturePerso("WAP_DT_NNS");
*/
/*
        addFeaturePerso("AVG_TO");
        addFeaturePerso("AVG_AND");
        addFeaturePerso("AVG_OF");
        addFeaturePerso("AVG_IN");
        addFeaturePerso("AVG_A");
        addFeaturePerso("AVG_IS");
        addFeaturePerso("AVG_THAT");
        addFeaturePerso("AVG_PEOPLE");
        addFeaturePerso("AVG_IT");
        addFeaturePerso("AVG_ARE");
        addFeaturePerso("AVG_THEY");
        addFeaturePerso("AVG_FOR");
        addFeaturePerso("AVG_HAVE");
        addFeaturePerso("AVG_NOT");
        addFeaturePerso("AVG_BE");
        addFeaturePerso("AVG_YOU");
        addFeaturePerso("AVG_MORE");
        addFeaturePerso("AVG_CAN");
        addFeaturePerso("AVG_WILL");
        addFeaturePerso("AVG_THEIR");
        addFeaturePerso("AVG_WITH");
        addFeaturePerso("AVG_DO");
        addFeaturePerso("AVG_THIS");
        addFeaturePerso("AVG_WE");
        addFeaturePerso("AVG_AS");
        addFeaturePerso("AVG_OR");
        addFeaturePerso("AVG_ON");
        addFeaturePerso("AVG_BUT");
        addFeaturePerso("AVG_IF");
        addFeaturePerso("AVG_LIFE");
        addFeaturePerso("AVG_THAN");
        addFeaturePerso("AVG_ONE");
        addFeaturePerso("AVG_YOUNG");
        addFeaturePerso("AVG_TIME");
        addFeaturePerso("AVG_THERE");
        addFeaturePerso("AVG_BY");
        addFeaturePerso("AVG_SO");
        addFeaturePerso("AVG_BECAUSE");
        addFeaturePerso("AVG_MY");
        addFeaturePerso("AVG_ABOUT");
        addFeaturePerso("AVG_HE");
*/
        /*** Initialisation de la matrice avec des 0 ***/
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));
        resetConfusionMatrix();
    }
}

/* Transformer m_featuresDico en local */
void Model::addFeaturePerso(const string &f){
    m_featuresDico.emplace(f + "_INF", m_featuresDico.size());
    m_featuresDico.emplace(f + "_SUP", m_featuresDico.size());
    m_featuresDico.emplace(f + "_BET", m_featuresDico.size());
}

/* Transformer langMatrix en local */
void Model::corrigerMatrice(const float& forceCorrection, const size_t& langMoins, const size_t& langPlus, const set<int> &foundFeatures)
{
    for(int ff : foundFeatures)
    {
        m_langMatrix[ff][langMoins] -= forceCorrection;
        m_langMatrix[ff][langPlus] += forceCorrection;
    }
}

void Model::saveFeaturesDictionnary() const throw()
{
    cout << m_featuresPath << endl;
    cout << ">> Sauvegarde de " << m_featuresDico.size() << " caracteristiques\n\n";

    ofstream cFile(m_featuresPath, ios::out);

    if(!cFile)
    {
        throw runtime_error(string("Erreur lors de la sauvegarde,"
                                   "impossible d'ouvrir le fichier " + m_featuresPath + "\n"));
        return;
    }
    cFile << m_featuresDico.size() << endl;
    for(auto &a: m_featuresDico)
    {
        /* ecrire chaque mot unique sous la forme d'une caractéristique */
        cFile << a.first << endl;
    }
    cFile.close();
}

void Model::saveLangMatrix() const throw()
{
    cout << m_langMatrixPath << endl;
    cout << ">> Sauvegarde de la matrice des langues [ " << m_featuresDico.size() << " * " << m_languages.size() << " ]\n\n";

    ofstream mFile(m_langMatrixPath, ios::out);

    if(!mFile)
    {
        throw runtime_error(string("Erreur lors de la sauvegarde,"
                                   "impossible d'ouvrir le fichier " + m_langMatrixPath + "\n"));
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

void Model::loadFeaturesDictionnary() throw()
{
    cout << ">> Extraction des caracteristiques\n";

    ifstream cFile(m_featuresPath, ios::in);

    if(!cFile)
    {
        throw runtime_error(string("Erreur lors du chargement,"
                                   "impossible d'ouvrir le fichier " + m_featuresPath + "\n"));
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

void Model::loadLangDictionnary() throw()
{
    cout << ">> Extraction du modele\n";
    ifstream mFile(m_langMatrixPath, ios::in);

    if(!mFile)
    {
        throw runtime_error(string("Erreur lors de la sauvegarde,"
                                   "impossible d'ouvrir le fichier " + m_langMatrixPath + "\n"));
        return;
    }
    size_t nbLangues, nbLignes;
    mFile >> nbLangues >> nbLignes;

    set<string> langSet;
    string lang;

    /*** Extraction de la liste des langues ***/
    size_t l=0;
    for(l=0; l<nbLangues; l++)
    {
        mFile >> lang;
        langSet.emplace(lang);
    }
    m_languages = Tools::setToVector(langSet);
    cout << "(" << nbLangues << " langues) * (" << nbLignes << " lignes)\n\n";

    string ligne, score;
    vector<vector<float>> langMatrix(nbLignes);
    for(size_t i=0; i<nbLignes; i++)
    {
        for(l=0; l<nbLangues; l++)
        {
            mFile >> score;
            langMatrix[i].push_back(atof(score.c_str()));
        }
    }
    mFile.close();

    m_langMatrix = langMatrix;
}
