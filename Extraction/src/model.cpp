#include <iostream>
#include <fstream>
#include <sstream>

#include "tools.h"
#include "model.h"
#include "essay.h"

using namespace std;

// Constantes
const unsigned int Model::AVG_WORD_CORPUS[2] = {324, 373};
const unsigned int Model::AVG_LETTER_CORPUS[2] = {1653, 1942};
const float Model::AVG_S_WORD[2] = {5.08, 5.23};
const unsigned int Model::AVG_WEND_ING[2] = {8, 11};
const unsigned int Model::AVG_FIRSTCAPS[2] = {18, 23};
const unsigned int Model::AVG_I_CAPS[2] = {2, 5};
const unsigned int Model::AVG_I_LOW[2] = {0, 1};
const unsigned int Model::AVG_PRONOMS[2] = {16, 19};
const unsigned int Model::AVG_THE[2] = {13, 18};
const unsigned int Model::AVG_S_SENTENCE[2] = {19, 25};
const unsigned int Model::AVG_POINT[2] = {13, 18};
const unsigned int Model::AVG_COMMA[2] = {10, 16};



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

Model::~Model()
{
    //dtor
}

/*** Méthodes publiques ***/

/* Cette fonction est encore utile?
 * Amirali
 */
void Model::trainAll()
{
    train(m_corpusList);
}

float Model::trainByDiv(const size_t& nbDiv, const size_t& stop)
{
    auto engine = std::default_random_engine{};
    vector<Essay> trainCorpus;
    vector<Essay> testCorpus;

    float err[nbDiv];
    size_t n = nbDiv;
    /*** Lancement de l'entrainement en cross validation ***/
    for(size_t numTrain=0; numTrain<nbDiv; ++numTrain)
    {

        cout<<"=== Train "<<numTrain+1<<" / "<<nbDiv<<" ===\n";
        // reset avec des 0 partout
        m_langMatrix.clear();
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));

        trainCorpus.clear();
        testCorpus.clear();

        int l[m_languages.size()]; for(size_t i=0; i<m_languages.size(); i++){ l[i] = 0; }
        /*** Sélection du corpus de textes pour la cross validation ***/
        for(size_t i=0; i<m_corpusList.size(); i++)
        {
            int indexLang = Tools::getVectorIndex(m_languages, m_corpusList[i].getLang());
            if(l[indexLang]%nbDiv == numTrain)
            {
                testCorpus.push_back(m_corpusList[i]);
            }
            else
            {
                trainCorpus.push_back(m_corpusList[i]);
            }
            l[indexLang]++;
        }

        /*** Entrainement du corpus d'entrainement ***/
        train(trainCorpus);

        /* estimation du taux d'erreur sur ce corpus */
        int nbErrors = 0;

        /*** Shuffle et test du programme sur le corpus de test ***/
        /*******************
         Shuffle marche pas
        *******************/
//        if(numTrain==0) cout << "Avant shuffle :\n" << testCorpus[0] << endl;
        std::shuffle(std::begin(testCorpus), std::end(testCorpus), engine);
//        if(numTrain==0) cout << "Apres shuffle :\n" << testCorpus[0] << endl;
        for(vector<Essay>::iterator it=testCorpus.begin(); it!=testCorpus.end(); ++it)
        {
            if( ! evaluer(*it)){
                nbErrors++;
            }
        }
        err[numTrain] = ((float)nbErrors / (float)testCorpus.size()) * (float)100;

        cout << "Erreurs : " << nbErrors << " / " << testCorpus.size() << " (" << err[numTrain] << " %)\n\n";

        /* arreter avant la fin */
        if(numTrain == stop-1)
        {
            n = stop;
            break;
        }
    }

    /* Reset de la matrice (0 partout) */
    m_langMatrix.clear();
    m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));

    float errMoy = Tools::floatArrayAVG(err, n);
    cout << "Moyenne erreurs = " << errMoy << " %\n\n" ;


    /*** Sauvegarde des résultats de chaque train dans un fichier ***/
    string path = "data/history.txt";
    cout << path << endl;
    cout << ">> Sauvegarde des resultats de " << n << " entrainement" << (n>1?"s":"") << "\n\n";

    ofstream cFile(path, ios::out | ios::app);
    if(!cFile)
    {
        //throw une exception
        return -1;
    }
    for(size_t i=0; i<n; i++)
    {
        cFile << "Train " << i << " = " << err[i] << " %\n";
    }
    cFile << "Moyenne = " << errMoy << endl;
    cFile.close();

    return errMoy;
}

//retourne l'indice de la langue
bool Model::evaluer(Essay &e, const float& forceCorrection)
{
    set<int> found;

    const vector<string> wordsList = e.getWordsList();
    /*** Caractéristiques personnalisées ***/

    size_t corpusSize = wordsList.size();
    unsigned int nbPhrases = e.getNbSentences();

    /** Nb de mots dans l'essai **/
    /** 1 = pire / 2 = mieux / 3 = mieux + **/
    evaluerFeature(corpusSize, AVG_WORD_CORPUS, "AVG_WORD_CORPUS", found, 3); //2

    /** Nb de lettres dans l'essai **/
    /** 1 = pire / 2 = mieux + / 3 = mieux ++ **/
    evaluerFeature(e.getTextSize(), AVG_LETTER_CORPUS, "AVG_LETTER_CORPUS", found, 3); //2

    /** Moyenne de lettres par mots **/
    /** 1 = rien / 2 = rien / 3 = rien **/
    evaluerFeature(e.getSizeWord(), AVG_S_WORD, "AVG_S_WORD", found, 0); //0

    /** nb de mots finissant par ING **/
    /** 1 = mieux ++ / 2 = mieux + / 3 = pire **/
    evaluerFeature(e.getNbFinishING(), AVG_WEND_ING, "AVG_WEND_ING", found, 1); //3

    /*** nb de mots commençant par une majuscule ***/
    /** 1 = mieux + / 2 = mieux ++ / 3 = mieux **/
    evaluerFeature(e.getNbFirstCaps(), AVG_FIRSTCAPS, "AVG_FIRSTCAPS", found, 2); //2

    /*** nb de mots "I" (en majuscule) ***/
    /** 1 = pire / 2 = mieux(rien) / 3 = mieux(quasi rien) **/
    evaluerFeature(e.getNbI(), AVG_I_CAPS, "AVG_I_CAPS", found, 0); //2

    /*** nb de mots "i" (en minuscule) ***/
    /** 1 = pire ++ / 2 = pire ++ / 3 = rien **/
    evaluerFeature(e.getNbi(), AVG_I_LOW, "AVG_I_LOW", found, 0); //2

    /*** nb de pronoms "i,you,he,she,it,we,they" (majuscule ou minuscule) ***/
    /** 1 = mieux -- / 2 = mieux / 3 = mieux - **/
    evaluerFeature(e.getNbPronoms(), AVG_PRONOMS, "AVG_PRONOMS", found, 2); //2

    /*** nb de "the" ***/
    /** 1 = pire / 2 = pire + / 3 = pire(presque rien) **/
    evaluerFeature(e.getNbThe(), AVG_THE, "AVG_THE", found, 0); //2

    /*** nb de mots par phrase **/
    /** 1 = mieux + / 2 = mieux ++ / 3 = mieux **/
    evaluerFeature(corpusSize/(float)nbPhrases, AVG_S_SENTENCE, "AVG_S_SENTENCE", found, 2); //0

    /*** nb de phrases (nb de points) **/
    /** 1 = mieux + / 2 = mieux ++ / 3 = mieux **/
    evaluerFeature(nbPhrases, AVG_POINT, "AVG_POINT", found, 2); //0

    /*** nb de virgules **/
    /** 1 = pire +++ / 2 = pire ++ / 3 = mieux **/
    evaluerFeature(e.getNbComma(), AVG_COMMA, "AVG_COMMA", found, 3); //0





    for(size_t i=0; i<corpusSize-1; i++)
    {
        /*** Caractéristiques sur les mots ***/
        addIfFound(found, "NB_W_" + wordsList[i]);
        /*** Caractéristiques sur les paires de mots ***/
        if(forceCorrection > 0)
        {   /* si entrainement en cours, ajouter la paire de mots */
            addFeatureAndResize("NB_2W_" + wordsList[i] + "_" + wordsList[i+1]);
        }
        /* si evaluation sans amelioration de l'entrainement */
        addIfFound(found, "NB_2W_" + wordsList[i] + "_" + wordsList[i+1]);
    }
    /*** dernier mot ***/
    addIfFound(found, "NB_W_" + wordsList[corpusSize-1]);



    /*** Calcul du score de chaque langue ***/
    int score[m_languages.size()] = {0};
    for(int i : found)
    {
        for(size_t j=0; j<m_languages.size(); j++)
        {
            score[j] += m_langMatrix[i][j];
        }
    }

    /** avoir le numero supose de la langue du texte **/
    int lang = Tools::getMaxIndex(score, m_languages.size());
    /** avoir le numero de la langue du texte **/
    int numLangText = Tools::getVectorIndex(m_languages, e.getLang());
    /*** ajouter a la matrice de confusion ***/
    if(forceCorrection == 0)
    {   /* lorsqu'il y a evaluation sans amelioration de l'entrainement */
        m_confusionMatrix[lang][numLangText] += 1;
    }
    if(m_languages[lang] != e.getLang())
    {
         if(forceCorrection > 0)
        {
            corrigerMatrice(forceCorrection, lang, numLangText, found);
        }
        return false;
    }
    return true;
}

/** unsigned int features **/
void Model::evaluerFeature(const unsigned int &val, const unsigned int borne[], const string &featureName, set<int> &found, const unsigned int &mode){
    if(mode==1 || mode==2)
    {
        if(val <= borne[0]){
            addIfFound(found, featureName + "_INF");
        }
        else if(val >= borne[1])
        {
            addIfFound(found, featureName + "_SUP");
        }
    }
    if(mode==2 || mode==3){
        if(val > borne[0] && val < borne[1])
        {
            addIfFound(found, featureName + "_BET");
        }
    }
}

/** float features **/
void Model::evaluerFeature(const float &val, const float borne[], const string &featureName, set<int> &found, const unsigned int &mode){
    if(mode==1 || mode==2)
    {
        if(val <= borne[0]){
            addIfFound(found, featureName + "_INF");
        }
        else if(val >= borne[1])
        {
            addIfFound(found, featureName + "_SUP");
        }
    }
    if(mode==2 || mode==3){
        if(val > borne[0] && val < borne[1])
        {
            addIfFound(found, featureName + "_BET");
        }
    }
}

void Model::corrigerMatrice(float forceCorrection, size_t langMoins, size_t langPlus, set<int> &foundFeatures)
{
    for(int ff : foundFeatures)
    {
        m_langMatrix[ff][langMoins] -= forceCorrection;
        m_langMatrix[ff][langPlus] += forceCorrection;
    }
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
    //System("../script/script.sh");
    //ifstream labeledCorpusFile("../script/trainTagLine.txt", ios::in);
    //ifstream labeledOcc1CorpusFile("../script/occurence1TagLine.txt", ios::in);
    //ifstream labeledOcc2CorpusFile("../script/occurence2TagLine.txt", ios::in);
    //ifstream labeledOcc3CorpusFile("../script/occurence3TagLine.txt", ios::in);

    ifstream corpusFile(m_trainPath, ios::in);
    if(!corpusFile /* || !labeledCorpusFile || !labeledOcc1CorpusFile || !labeledOcc2CorpusFile || !labeledOcc3CorpusFile */)
    {
       cerr << "Impossible d'ouvrir un des fichiers !" << endl;
    }
    else
    {
        cout << m_trainPath << endl;
        cout << ">> Lecture du fichier" << endl << endl;

        /*** Lecture du corpus et enregistrement des caractéristiques ***/
        set<string> langSet;
        //map<string, int> tempFeatures;
        string line/*, labeledline*/;
        /* Attention ici mettre des && entre les getlines n'est pas correct ! On sortirait du while
         * dès que l'on aurait fini de lire un des fichiers
         * Peut être mieux de lire les fichiers les uns après les autres?
         * Amirali
         */
        while (getline(corpusFile, line) /* && getline(labeledCorpusFile, labeledLine) && getline(labeledOcc1CorpusFile, labeledOcc1Line)
            && getline(labeledOcc2CorpusFile, labeledOcc2Line) && getline(labeledOcc3CorpusFile, labeledOcc3Line)*/)
        {
            try
            {
                Essay e(line/*, labeledLine, labeledOcc1Line, labeledOcc2Line, labeledOcc3Line*/, m_featuresDico, langSet);
                m_corpusList.push_back(e);
            }catch(const invalid_argument& e)
            {
                cerr << e.what() << endl;
            }
        }
        corpusFile.close();
        m_languages = Tools::setToVector(langSet);

        /* Je vois pas trop l'utilité de copier le dico d'une variable temp au champ de classe
         * Autant direct mettre les features dans le champ de la classe m_featuresDico
         * Amirali
         */

        cout << "featuresWords size: " << m_featuresDico.size() << endl;


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


        /*** Initialisation de la matrice avec des 0 ***/
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));
        resetConfusionMatrix();
    }
}

void Model::train(vector<Essay> &corpus)
{
    size_t i=0, x=10;
    float forceCorrection = 1;
    int nbErrors = 1;

    while(i<x && nbErrors>0)
    { /* Condition d'arrêt : x = nombre de tours max OU zero erreur au tour précédent */
        nbErrors = 0;
        forceCorrection = (float)1/(float)(i+1);

        /* evaluer chaque essay de train
         * shuffle trainCorpus
         */
        random_shuffle(corpus.begin(), corpus.end());
        for(size_t j=0; j<corpus.size(); j++)
        {
            if( ! evaluer(corpus[j], forceCorrection))
            {
                nbErrors++;
            }
        }

        float err = ((float)nbErrors / (float)corpus.size()) * (float)100;
        cout << "Step " << i << " - Erreurs : " << nbErrors << " / " << corpus.size() << " (" << err << " %)\n";

        i++;
    }
}

void Model::addFeaturePerso(const string &f){
    m_featuresDico.emplace(f + "_INF", m_featuresDico.size());
    m_featuresDico.emplace(f + "_SUP", m_featuresDico.size());
    m_featuresDico.emplace(f + "_BET", m_featuresDico.size());
}

void Model::addFeatureAndResize(const string &f){
    try{
        m_featuresDico.at(f);
    }
    catch(const out_of_range &oor){
        m_featuresDico.emplace(f, m_featuresDico.size());
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));
    }
}

void Model::addIfFound(set<int> &found, const string &feature)
{
    map<string, int>::iterator it = m_featuresDico.find(feature);
    if(it!=m_featuresDico.end()) found.emplace(it->second);
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
