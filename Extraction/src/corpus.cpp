#include "corpus.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>
#include <thread>

#include "tools.h"

using namespace std;


/*** constructeur de cross-validation ***/
Corpus::Corpus(const std::vector<std::string>& languages,
               std::map<std::string, int>& featuresDico,
              /*const?*/ std::vector<Essay>& trainingCorpusList, const size_t& nbDiv, const size_t& numTrain)
    : m_numTrain(numTrain), m_languages(languages), m_featuresDico(featuresDico),
      m_langMatrix(), m_confusionMatrix(), m_trainCorpus(), m_testCorpus()
{
    m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_languages.size()));
    resetConfusionMatrix();

    int l[m_languages.size()] = {0};
    /*** Sélection du corpus de textes pour la cross validation ***/
    for(size_t i=0; i<trainingCorpusList.size(); i++)
    {
        int indexLang = Tools::getVectorIndex(m_languages, trainingCorpusList[i].getLang());
        if(l[indexLang]%nbDiv == m_numTrain)
        {
            m_testCorpus.push_back(trainingCorpusList[i]);
        }
        else
        {
            m_trainCorpus.push_back(trainingCorpusList[i]);
        }
        l[indexLang]++;
    }
}

Corpus::Corpus(const std::vector<std::string>& languages, std::map<std::string, int>& featuresDico,
               std::vector<Essay>& testCorpusList)
    : m_numTrain(0), m_languages(languages), m_featuresDico(featuresDico),
      m_langMatrix(), m_confusionMatrix(), m_trainCorpus(), m_testCorpus(testCorpusList)
{
    resetConfusionMatrix();
}

Corpus::Corpus(const Corpus& other)
    : m_numTrain(other.m_numTrain), m_languages(other.m_languages), m_featuresDico(other.m_featuresDico),
    m_langMatrix(other.m_langMatrix), m_confusionMatrix(other.m_confusionMatrix), m_trainCorpus(other.m_trainCorpus), m_testCorpus(other.m_testCorpus)
{
    //copy ctor
}

float Corpus::train(bool verbose)
{
    size_t i=0, x=10;
    float forceCorrection = 1.0f;
    int nbErrors = 1;
    int nbErrTest;

    /* Entrainement */
    while(i<x && nbErrors>0)
    { /* Condition d'arrêt : x = nombre de tours max OU zero erreur au tour précédent */
        nbErrors = 0;
        forceCorrection = 1.0f/static_cast<float>(i+1);

        /* evaluer chaque essay de trainCorpus
         */
        nbErrors = evaluer(m_trainCorpus, forceCorrection);

        if(verbose)
        {
            float error = (static_cast<float>(nbErrors) / static_cast<float> (m_trainCorpus.size())) * 100.0f;
            cout << "Step " << i << " - Erreurs : " << nbErrors << " / " << m_trainCorpus.size() << " (" << error << " %)\n";
        }
        i++;
    }

    /* Test */
    nbErrTest = evaluer(m_testCorpus);
    /* Pourcentage */
    float errorPer = (static_cast<float>(nbErrTest) / static_cast<float>(m_testCorpus.size())) * 100.0f;

    cout << "Erreurs (entrainement nb:" << m_numTrain+1 << ", idThread: " << this_thread::get_id() << ") : " << nbErrTest << " / " << m_testCorpus.size() << " (" << errorPer << " %)\n\n";

    return errorPer;
}

void Corpus::resetConfusionMatrix() {
    /** initialisation de la matrice de confusion a 0 partout */
    m_confusionMatrix.clear();
    m_confusionMatrix.resize(m_languages.size(), vector<int>(m_languages.size()));
}

void Corpus::printConfusionMatrix(const string& path) const{
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
int Corpus::evaluer(vector<Essay>& subCorpus, const float& forceCorrection)
{
    int nbErrors = 0;
    size_t subCorpusSize = subCorpus.size();

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(subCorpus.begin(), subCorpus.end(), std::default_random_engine(seed));
    //random_shuffle(subCorpus.begin(), subCorpus.end());

    for(size_t j=0; j<subCorpusSize; j++)
    {
        Essay& currentEssay = subCorpus[j];
        set<int> featuresFound;

        if(m_langMatrix.size() == 0)
        {
            cout << "Bug dans corpus::evaluer" << endl;
            this_thread::sleep_for(std::chrono::seconds(30));
        }
        size_t langFoundInd = currentEssay.evaluer(m_languages.size(), m_featuresDico, m_langMatrix, featuresFound);
        string trueLang = currentEssay.getLang();
        int numLangText = Tools::getVectorIndex(m_languages, trueLang);

        /*** ajouter a la matrice de confusion ***/
        if(forceCorrection == 0)
        {   /* lorsqu'il y a evaluation sans amelioration de l'entrainement */
            m_confusionMatrix[langFoundInd][numLangText] ++;
        }

        if(numLangText != langFoundInd)
        {
            if(forceCorrection > 0)
            {
                corrigerMatrice(forceCorrection, langFoundInd, numLangText, featuresFound);
            }
            nbErrors++;
        }
    }
    return nbErrors;
}

void Corpus::addIfFound(set<int>& found, const string& feature)
{
    /* Peut être bug avec auto map<string, int>::iterator
     * Amirali
     */
    auto it = m_featuresDico.find(feature);
    if(it!=m_featuresDico.end()) found.emplace(it->second);
}

void Corpus::corrigerMatrice(const float& forceCorrection, const size_t& langMoins, const size_t& langPlus, const set<int>& foundFeatures)
{
    for(const size_t& ff : foundFeatures)
    {
        m_langMatrix[ff][langMoins] -= forceCorrection;
        m_langMatrix[ff][langPlus] += forceCorrection;
    }

}

//Corpus& Corpus::operator=(const Corpus& rhs)
//{
//    if (this == &rhs) return *this; // handle self assignment
//    //assignment operator
//    return *this;
//}
