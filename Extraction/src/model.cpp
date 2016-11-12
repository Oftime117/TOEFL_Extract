#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>

#include "tools.h"
#include "model.h"
#include "essay.h"
#include "corpus.h"

using namespace std;

// Constantes

const unsigned int Model::SZ_CORPUS_INF = 326;
const unsigned int Model::SZ_CORPUS_SUP = 372;
const unsigned int Model::NB_LETTER_INF = 1660;
const unsigned int Model::NB_LETTER_SUP = 1936;
const float Model::SZ_WORD_INF = 5.08;
const float Model::SZ_WORD_SUP = 5.23;


Model::Model(string corpusPath, string featuresOut, string langMatrixOut)
    : m_trainPath(corpusPath), m_featuresPath(featuresOut), m_langMatrixPath(langMatrixOut),
    m_languages(), m_featuresDico(), m_langMatrix(), m_corpusList()
{
    initModel();
}

/* pourquoi trainPath et corpusList ne sont pas initialisé?
 * Amirali
 */
Model::Model(string featuresIn, string langMatrixIn):m_featuresPath(featuresIn),
m_langMatrixPath(langMatrixIn), m_languages(), m_featuresDico(), m_langMatrix()
{
    loadFeaturesDictionnary();
    loadLangDictionnary();
}

/* Pourquoi les autres variables ne sont pas initialisé?
 * Amirali
 */
Model::Model(vector<Essay> &corpusList):m_corpusList(corpusList)
{

    cout<<"m_corpusList taille ==== "<<m_corpusList.size()<<endl;

    // Initialisation de la matrice avec des 0
    m_langMatrix = vector<vector<float> >(m_featuresDico.size(), vector<float>(m_languages.size(), 0));
}


/*** Méthodes publiques ***/

/* Cette fonction est encore utile?
 * Amirali
 */
void Model::trainAll()
{
    train(m_corpusList);
}

float Model::trainByDiv3(const size_t& nbDiv)
{
    float err[nbDiv];
    vector<future<float> > futures;
    vector<Corpus> corpusTab;
    /*** Lancement de l'entrainement en cross validation ***/
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
            err[i] = fut.get();
            i++;
        }
        float errMoy = Tools::floatArrayAVG(err, nbDiv);
        cout << "Moyenne erreurs = " << errMoy << " %\n\n" ;
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
    vector<future<float> > futures;
    /*** Lancement de l'entrainement en cross validation ***/
    for(size_t numTrain=0; numTrain<nbDiv; ++numTrain)
    {

        cout<<"=== Train "<<numTrain+1<<" / "<<nbDiv<<" ===\n";

        Corpus trainCorpus(m_languages, m_featuresDico, m_corpusList, nbDiv, numTrain);

        err[numTrain] = trainCorpus.train();
    }

    float errMoy = Tools::floatArrayAVG(err, nbDiv);
    cout << "Moyenne erreurs = " << errMoy << " %\n\n" ;


    /*** Sauvegarde des résultats de chaque train dans un fichier ***/
    size_t pos = m_featuresPath.find(".");
    string path = m_featuresPath.substr(0, pos) + "_train.txt";
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

float Model::trainByDiv(const size_t& nbDiv)
{
    vector<Essay> trainCorpus;
    vector<Essay> testCorpus;

    float err[nbDiv];
    /*** Lancement de l'entrainement en cross validation ***/
    for(size_t numTrain=0; numTrain<nbDiv; ++numTrain)
    {

        cout<<"=== Train "<<numTrain+1<<" / "<<nbDiv<<" ===\n";
        // reset avec des 0 partout
        m_langMatrix.clear();
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));

        trainCorpus.clear();
        testCorpus.clear();

        /*** Sélection du corpus de textes pour la cross validation ***/
        for(size_t i=0; i<m_corpusList.size(); i++)
        {
            if(i%nbDiv == numTrain)
            {
                testCorpus.push_back(m_corpusList[i]);
            }
            else
            {
                trainCorpus.push_back(m_corpusList[i]);
            }
        }

        /*** Entrainement du corpus d'entrainement ***/
        train(trainCorpus);

        /* estimation du taux d'erreur sur ce corpus */

        /*** Shuffle et test du programme sur le corpus de test ***/
        /* Placer la suite dans une fonction?
         * Le même bout de code apparait dans la méthode train
         * Amirali
         */
        int nbErrors = 0;
        random_shuffle(testCorpus.begin(), testCorpus.end());
        for(size_t j=0; j<testCorpus.size(); j++)
        {
            if( ! evaluer(testCorpus[j]))
            {
                nbErrors++;
            }
        }
        err[numTrain] = ((float)nbErrors / (float)testCorpus.size()) * (float)100;

        cout << "Erreurs : " << nbErrors << " / " << testCorpus.size() << " (" << err[numTrain] << " %)\n\n";
    }

    /* Reset de la matrice (0 partout) */
    m_langMatrix.clear();
    m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));

    float errMoy = Tools::floatArrayAVG(err, nbDiv);
    cout << "Moyenne erreurs = " << errMoy << " %\n\n" ;


    /*** Sauvegarde des résultats de chaque train dans un fichier ***/
    size_t pos = m_featuresPath.find(".");
    string path = m_featuresPath.substr(0, pos) + "_train.txt";
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
        cout << ">> Lecture du fichier et initialisation des features" << endl << endl;

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
                m_corpusList.push_back(std::move(e));
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
//        m_featuresDico = map<string, int>();
//        for(const auto& t: tempFeatures)
//        {
//            m_featuresDico.emplace(t.first, t.second);
//        }
        cout << "featuresWords size: " << m_featuresDico.size() << endl;

        /*** Paire de mots ***/
//        for(const auto& t1 : tempFeatures){
//            string s1 = t1.first.substr(5);
//            for(const auto& t2 : tempFeatures){
//                string s2 = "NB_2W_" + s1 + "_" + t2.first.substr(5);
//                m_featuresDico.emplace(s2, m_featuresDico.size());
//            }
//        }


        /*** Ajout des caractéristiques personnalisées ***/

        /* Créer variable locale pour la taille du dico,
         * Appeler size tout le temps est pas opti
         * Amirali
         */
        m_featuresDico.emplace("SZ_CORPUS_INF", m_featuresDico.size());
        m_featuresDico.emplace("SZ_CORPUS_SUP", m_featuresDico.size());
        m_featuresDico.emplace("NB_LETTER_INF", m_featuresDico.size());
        m_featuresDico.emplace("NB_LETTER_SUP", m_featuresDico.size());
        m_featuresDico.emplace("SZ_WORD_INF", m_featuresDico.size());
        m_featuresDico.emplace("NB_WORD_SUP", m_featuresDico.size());

        /*** Initialisation de la matrice avec des 0 ***/
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));
    }
    /* Pas utile
     * Amirali
     */
    corpusFile.close();
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

//retourne l'indice de la langue
bool Model::evaluer(Essay &e, const float& forceCorrection)
{
    set<int> found;

    const vector<string> wordsList = e.getWordsList();
    /*** Caractéristiques personnalisées ***/
    /* Il faudrait une troisième borne pour chaque,
     * deux ça suffit pas, il faut mettre la caractéristique pour quand
     * c'est compris entre les deux valeurs
     * Amirali
     */
    size_t corpusSize = wordsList.size();
    if(corpusSize <= SZ_CORPUS_INF)
        addIfFound(found, "SZ_CORPUS_INF");
    else if(corpusSize >= SZ_CORPUS_SUP)
        addIfFound(found, "SZ_CORPUS_SUP");

    /* ça ne vas pas du tout ça, on perd du temps
     * à charger le texte juste pour avoir la taille
     * De plus, size() donne la taille du texte entier, du coup ponctuation et espaces compris
     * du coup mauvais calcul vu qu'il compte les ponctuations et espaces comme des lettres.
     * Amirali
     */
    if(e.getText().size() <= NB_LETTER_INF)
        addIfFound(found, "NB_LETTER_INF");
    else if(e.getText().size() >= NB_LETTER_SUP)
        addIfFound(found, "NB_LETTER_SUP");

    if(e.getSizeWord() <= NB_LETTER_INF)
        addIfFound(found, "SZ_WORD_INF");
    else if(e.getSizeWord() >= NB_LETTER_SUP)
        addIfFound(found, "SZ_WORD_SUP");

    /*** Caractéristiques sur les mots ***/
    for(size_t i=0; i<corpusSize; i++)
    {
        addIfFound(found, "NB_W_" + wordsList[i]);
    }

    /*** Caractéristiques sur les paires de mots ***/
    for(size_t i=0; i<corpusSize-1; i++)
    {
        addIfFound(found, "NB_2W_" + wordsList[i] + "_" + wordsList[i+1]);
    }



    /*** Calcul du score de chaque langue ***/
    int score[m_languages.size()] = {0};
    for(int i : found)
    {
        for(size_t j=0; j<m_languages.size(); j++)
        {
            score[j] += m_langMatrix[i][j];
        }
    }

    int lang = Tools::getMaxIndex(score, m_languages.size());
    if(m_languages[lang] != e.getLang())
    {
        //avoir le numero de la langue du texte
        int numLangText = Tools::getVectorIndex(m_languages, e.getLang());
        if(forceCorrection > 0)
        {
            corrigerMatrice(forceCorrection, lang, numLangText, found);
        }
        return false;
    }
    return true;
}

/* Transformer m_featuresDico en local */
void Model::addIfFound(set<int> &found, const string &feature)
{
    map<string, int>::iterator it = m_featuresDico.find(feature);
    if(it!=m_featuresDico.end()) found.emplace(it->second);
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
