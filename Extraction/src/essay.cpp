#include "essay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>

#include "tools.h"

using namespace std;

const unsigned int Essay::SZ_CORPUS_INF = 326;
const unsigned int Essay::SZ_CORPUS_SUP = 372;
const unsigned int Essay::NB_LETTER_INF = 1660;
const unsigned int Essay::NB_LETTER_SUP = 1936;
const float Essay::SZ_WORD_INF = 5.08;
const float Essay::SZ_WORD_SUP = 5.23;


Essay::Essay(const string& essay, /* const string& labels, const string& labelsOcc1, const string& labelsOcc2, const string& labelsOcc3,*/ map<string, int>& dic, std::set<std::string> &langDico) throw()
{
    size_t firstSpace = essay.find_first_of(' ', 0); // (LANGUE,NIVEAU)
    size_t firstComa = essay.find_first_of(',', 0);
    size_t firstCP = essay.find_first_of(')', 0);

    // gestion d'erreur
    if(firstSpace == string::npos || firstComa == string::npos || firstCP == string::npos)
        throw invalid_argument(string("Erreur lors de la lecture du fichier text "+ essay + "\nFichier inccorect."));

    m_lang = essay.substr(1, firstComa-1);
    m_level = essay.substr(firstComa+1, firstCP-firstComa-1);
    string text = essay.substr(firstSpace+1); // texte sans langue et niveau
    //m_labels = labels.substr(firstSpace+1); // l'entête est la même
    //m_labelsOcc1 = labelsOcc1.substr(firstSpace+1);
    //m_labelsOcc2 = labelsOcc2.substr(firstSpace+1);
    //m_labelsOcc3 = labelsOcc3.substr(firstSpace+1);
    m_nbSentences = 0;

    splitEssay(' ', dic, text);
    //spitLabels(' ', dic);
    //spitLabelsOcc1(' ', dic);
    //spitLabelsOcc2(' ', dic);
    //spitLabelsOcc3(' ', dic);
    langDico.emplace(m_lang);
}

//Copy ctor
Essay::Essay(const Essay& other) :m_text(other.m_text), m_sizeWord(other.m_sizeWord),
m_wordsList(other.m_wordsList), m_lang(other.m_lang), m_level(other.m_level)
{

}

/* À modifier suivant les champs de la classe ajoutés dans le future
 * Amirali
 */
Essay::Essay(Essay&& rvalue)
{
    m_text = move(rvalue.m_text);
    m_sizeWord = rvalue.m_sizeWord;
    m_nbSentences = rvalue.m_nbSentences;
    m_wordsList = move(rvalue.m_wordsList);
    m_labelsList = move(rvalue.m_labelsList);
    m_lang = move(rvalue.m_lang);
    m_level = move(rvalue.m_level);

    rvalue.m_sizeWord = 0.0f;
    rvalue.m_nbSentences = 0;
}

/*** Opérateurs ***/

/* À modifier suivant les champs de la classe ajoutés dans le future
 * Amirali
 */
Essay& Essay::operator=(Essay&& rvalue)
{
    if(this != &rvalue)
    {
        m_text = move(rvalue.m_text);
        m_sizeWord = rvalue.m_sizeWord;
        m_nbSentences = rvalue.m_nbSentences;
        m_wordsList = move(rvalue.m_wordsList);
        m_labelsList = move(rvalue.m_labelsList);
        m_lang = move(rvalue.m_lang);
        m_level = move(rvalue.m_level);

        rvalue.m_sizeWord = 0.0f;
        rvalue.m_nbSentences = 0;
    }
    return *this;
}

Essay& Essay::operator=(const Essay& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

/*** Méthodes publiques ***/

size_t Essay::evaluer(const size_t& nbLang, map<string, int> & featuresDico,
        vector<vector<float> >& langMatrix, set<int>& foundFeatures)
{
    //set<int> found;

    const vector<string> wordsList = getWordsList();
    /*** Caractéristiques personnalisées ***/
    /* Il faudrait une troisième borne pour chaque,
     * deux ça suffit pas, il faut mettre la caractéristique pour quand
     * c'est compris entre les deux valeurs
     * Amirali
     */
    size_t corpusSize = wordsList.size();
    if(corpusSize <= SZ_CORPUS_INF)
        addIfFound(foundFeatures, "SZ_CORPUS_INF", featuresDico);
    else if(corpusSize >= SZ_CORPUS_SUP)
        addIfFound(foundFeatures, "SZ_CORPUS_SUP", featuresDico);

    /* ça ne vas pas du tout ça, on perd du temps
     * à charger le texte juste pour avoir la taille
     * De plus, size() donne la taille du texte entier, du coup ponctuation et espaces compris
     * du coup mauvais calcul vu qu'il compte les ponctuations et espaces comme des lettres.
     * Amirali
     */
    if(m_text.size() <= NB_LETTER_INF)
        addIfFound(foundFeatures, "NB_LETTER_INF", featuresDico);
    else if(m_text.size() >= NB_LETTER_SUP)
        addIfFound(foundFeatures, "NB_LETTER_SUP", featuresDico);

    if(m_sizeWord <= NB_LETTER_INF)
        addIfFound(foundFeatures, "SZ_WORD_INF", featuresDico);
    else if(m_sizeWord >= NB_LETTER_SUP)
        addIfFound(foundFeatures, "SZ_WORD_SUP", featuresDico);

    /*** Caractéristiques sur les mots ***/
    for(size_t i=0; i<corpusSize; i++)
    {
        addIfFound(foundFeatures, "NB_W_" + wordsList[i], featuresDico);
    }

    /*** Caractéristiques sur les paires de mots ***/
    for(size_t i=0; i<corpusSize-1; i++)
    {
        addIfFound(foundFeatures, "NB_2W_" + wordsList[i] + "_" + wordsList[i+1], featuresDico);
    }



    /*** Calcul du score de chaque langue ***/
    if(langMatrix.size() == 0)
    {
        cout << "Bug dans essay::evaluer" << endl;
    }
    int score[nbLang] = {0};
    for(int i : foundFeatures)
    {
        for(size_t j=0; j<nbLang; j++)
        {
            score[j] += langMatrix[i][j];
        }
    }
    /*** Renvoi de l'indice de la langue avec le plus grand score ***/
    return Tools::getMaxIndex(score, nbLang);
}


/* Méthodes evaluer pour le test final / À améliorer pour retirer la création du set- Amirali */
 size_t Essay::evaluer(const size_t& nbLang, std::map<std::string, int>& featuresDico,
        std::vector<std::vector<float> >& langMatrix)
{
    set<int> dummy;
    return evaluer(nbLang, featuresDico, langMatrix, dummy);
}

/*** Méthodes privées ***/

void Essay::splitEssay(const char& delim, map<string, int>& dic, const string& text)
{
    istringstream ss(text);
    string item;
    /* Pareil, l'utilité de la variable temporaire?
     * Amirali
     */
    // vector<string> wordsList;
    int somme = 0;
    while (getline(ss, item, delim))
    {
        string buff = std::move(item);
        /* Faudrait peut être compter le nombre de majuscules avant de tolower
         * ça peut être une caractéristiques
         * Amirali
         */
        // Compter les points peut suffir à compter les phrases
        // Florian
        transform(buff.begin(), buff.end(), buff.begin(), ::tolower);
        dic.emplace("NB_W_" + buff, dic.size());
        if (buff.compare(".") == 0) m_nbSentences++;
        // somme peut servir à calculer le nombre de lettres dans l'essai
        // Florian
        /* C'est utile de savoir le nombre de lettre d'un texte?? Amirali */
        somme += buff.size();
        m_wordsList.push_back(std::move(buff));
    }
    //m_wordsList = wordsList;
    m_sizeWord = somme/(float)m_wordsList.size();
}

void Essay::addIfFound(set<int> &found, const string &feature, map<string, int>& featuresDico)
{
    map<string, int>::iterator it = featuresDico.find(feature);
    if(it!=featuresDico.end()) found.emplace(it->second);
}

/* Florian
void Essay::splitLabels(char delim, map<string, int>& dic)
{
    stringstream ss;
    ss.str(m_labels);
    string item;
    vector<string> labelsList;
    while (getline(ss, occurence, delim) && getline(ss, item, delim))
    {
        string buff1 = occurence;
        string buff2 = item;
        dic.emplace("NB_L_" + buff, dic.size());
        labelsList.push_back(buff);
    }
    m_labelsList = labelsList;
}

void Essay::spitLabelsOcc1(char delim, map<string, int>& dic)
{
    stringstream ss;
    ss.str(m_labelsOcc1);
    string item;
    map<string, int> labelsMap;
    while (getline(ss, occurence, delim) && getline(ss, item, delim))
    {
        string buff1 = occurence;
        string buff2 = item;
        dic.emplace("ORDRE_L1_" + buff2, dic.size());
        labelsMap.emplace(buff2, std::stoi(buff1, 10));
    }
    m_labelsMap = labelsMap;
}

void Essay::spitLabelsOcc2(char delim, map<string, int>& dic)
{
    stringstream ss;
    ss.str(m_labelsOcc2);
    string item;
    map<string, int> labelsMap;
    while (getline(ss, occurence, delim) && getline(ss, item1, delim) && getline(ss, item2, delim))
    {
        string buff1 = occurence;
        string buff2 = item1;
        string buff3 = item2;
        dic.emplace("ORDRE_L2_" + buff2 + "_" + buff3, dic.size());
        labelsMap.emplace(buff2 + "_" + buff3, std::stoi(buff1, 10));
    }
    m_labelsMap = labelsMap;
}

void Essay::spitLabelsOcc3(char delim, map<string, int>& dic)
{
    stringstream ss;
    ss.str(m_labelsOcc3);
    string item;
    map<string, int> labelsMap;
    while (getline(ss, occurence, delim) && getline(ss, item1, delim) && getline(ss, item2, delim) && getline(ss, item3, delim))
    {
        string buff1 = occurence;
        string buff2 = item1;
        string buff3 = item2;
        string buff4 = item3;
        dic.emplace("ORDRE_L3_" + buff2 + "_" + buff3 + "_" + buff4, dic.size());
        labelsMap.emplace(buff2 + "_" + buff3 + "_" + buff4, std::stoi(buff1, 10));
    }
    m_labelsMap = labelsMap;
}*/

ostream& operator<< (ostream& stream, const Essay& essay)
{
    stream << "Langue: " << essay.m_lang << "\nNiveau: " << essay.m_level << "\nWord count: " << essay.m_wordsList.size() << "\nSentence count: " << essay.m_nbSentences;
    return stream;
}
