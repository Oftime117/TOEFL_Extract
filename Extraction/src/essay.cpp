#include "essay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>

#include "tools.h"

using namespace std;

/*** Constantes ***/
const unsigned int Essay::AVG_WORD_CORPUS[2] = {324, 373};
const unsigned int Essay::AVG_LETTER_CORPUS[2] = {1653, 1942};
const float Essay::AVG_S_WORD[2] = {5.08, 5.23};
const unsigned int Essay::AVG_WEND_ING[2] = {8, 11};
const unsigned int Essay::AVG_FIRSTCAPS[2] = {18, 23};
const unsigned int Essay::AVG_I_CAPS[2] = {2, 5};
const unsigned int Essay::AVG_I_LOW[2] = {0, 1};
const unsigned int Essay::AVG_PRONOMS[2] = {16, 19};
const unsigned int Essay::AVG_THE[2] = {13, 18};
const unsigned int Essay::AVG_S_SENTENCE[2] = {19, 25};
const unsigned int Essay::AVG_POINT[2] = {13, 18};
const unsigned int Essay::AVG_COMMA[2] = {10, 16};


Essay::Essay(const string& essay, /* const string& labels, const string& labelsOcc1, const string& labelsOcc2, const string& labelsOcc3,*/ map<string, int>& dic, std::set<std::string> &langDico) throw()
{
    size_t firstSpace = essay.find_first_of(' ', 0); // (LANGUE,NIVEAU)
    size_t firstComa = essay.find_first_of(',', 0);
    size_t firstCP = essay.find_first_of(')', 0);

    // gestion d'erreur
    if(firstSpace == string::npos || firstComa == string::npos || firstCP == string::npos)
        throw invalid_argument(string("Erreur lors de la lecture du fichier text "
                                      + essay + "\nFichier inccorect."));

    m_lang = essay.substr(1, firstComa-1);
    m_level = essay.substr(firstComa+1, firstCP-firstComa-1);
    string text = essay.substr(firstSpace+1); // texte sans langue et niveau
    m_textSize = text.size();
    m_nbLetter = 0;
    //m_labels = labels.substr(firstSpace+1); // l'entête est la même
    //m_labelsOcc1 = labelsOcc1.substr(firstSpace+1);
    //m_labelsOcc2 = labelsOcc2.substr(firstSpace+1);
    //m_labelsOcc3 = labelsOcc3.substr(firstSpace+1);
    m_nbSentences = 0;
    m_nbFinishING = 0;
    m_nbFirstCaps = 0;
    m_nbI = 0;
    m_nbi = 0;
    m_nbPronoms = 0;
    m_nbThe = 0;
    m_nbComma = 0;

    splitEssay(' ', dic, text);
    //spitLabels(' ', dic);
    //spitLabelsOcc1(' ', dic);
    //spitLabelsOcc2(' ', dic);
    //spitLabelsOcc3(' ', dic);
    langDico.emplace(m_lang);
}

//Copy ctor
Essay::Essay(const Essay& other) :m_textSize(other.m_textSize), m_nbLetter(other.m_nbLetter), m_avgSizeWord(other.m_avgSizeWord),
    m_nbSentences(other.m_nbSentences), m_wordsList(other.m_wordsList),
    m_lang(other.m_lang), m_level(other.m_level),
    m_nbFinishING(other.m_nbFinishING), m_nbFirstCaps(other.m_nbFirstCaps),
    m_nbI(other.m_nbI), m_nbi(other.m_nbi), m_nbPronoms(other.m_nbPronoms),
    m_nbThe(other.m_nbThe), m_nbComma(other.m_nbComma)
{

}

/* À modifier suivant les champs de la classe ajoutés dans le future
 * Amirali
 */
Essay::Essay(Essay&& rvalue)
{
    m_textSize = rvalue.m_textSize;
    m_nbLetter = rvalue.m_nbLetter;
    m_avgSizeWord = rvalue.m_avgSizeWord;
    m_nbSentences = rvalue.m_nbSentences;
    m_wordsList = move(rvalue.m_wordsList);
    m_labelsList = move(rvalue.m_labelsList);
    m_lang = move(rvalue.m_lang);
    m_level = move(rvalue.m_level);

    rvalue.m_textSize = 0;
    rvalue.m_nbLetter = 0;
    rvalue.m_avgSizeWord = 0.0f;
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
        m_textSize = rvalue.m_textSize;
        m_nbLetter = rvalue.m_nbLetter;
        m_avgSizeWord = rvalue.m_avgSizeWord;
        m_nbSentences = rvalue.m_nbSentences;
        m_wordsList = move(rvalue.m_wordsList);
        m_labelsList = move(rvalue.m_labelsList);
        m_lang = move(rvalue.m_lang);
        m_level = move(rvalue.m_level);

        rvalue.m_textSize = 0;
        rvalue.m_nbLetter = 0;
        rvalue.m_avgSizeWord = 0.0f;
        rvalue.m_nbSentences = 0;
    }
    return *this;
}

Essay& Essay::operator=(const Essay& other)
{
    if (this == &other) return *this; // handle self assignment

    m_textSize = other.m_textSize;
    m_nbLetter = other.m_nbLetter;
    m_avgSizeWord = other.m_avgSizeWord;
    m_nbSentences = other.m_nbSentences;
    m_wordsList = other.m_wordsList;
    m_labelsList = other.m_labelsList;
    m_lang = other.m_lang;
    m_level = other.m_level;
    m_nbFinishING = other.m_nbFinishING;
    m_nbFinishING = other.m_nbFirstCaps;
    m_nbI = other.m_nbI;
    m_nbi = other.m_nbi;
    m_nbPronoms = other.m_nbPronoms;
    m_nbThe = other.m_nbThe;
    m_nbComma = other.m_nbComma;

    return *this;
}

/*** Méthodes publiques ***/
size_t Essay::evaluer(const size_t& nbLang, map<string, int> & featuresDico,
        vector<vector<float> >& langMatrix, set<int>& foundFeatures /* = set<int>() - Amirali */)
{
    //set<int> found;

    const vector<string> wordsList = getWordsList();
    /*** Caractéristiques personnalisées ***/
    size_t corpusSize = wordsList.size();

    /** Nb de mots dans l'essai **/
    /** 1 = pire / 2 = mieux / 3 = mieux + **/
    evaluerFeature(corpusSize, AVG_WORD_CORPUS, "AVG_WORD_CORPUS", foundFeatures, featuresDico, 3); //2

    /** Nb de lettres dans l'essai **/
    /** 1 = pire / 2 = mieux + / 3 = mieux ++ **/
    /* Attention ici Vincent, en mettant m_text.size() on compte le nombre de ponctuation et d'espaces avec.
     * j'ai changé avec la somme des lettres m_nbLetter
     * Amirali
     */
    evaluerFeature(m_nbLetter, AVG_LETTER_CORPUS, "AVG_LETTER_CORPUS", foundFeatures, featuresDico, 3); //2

    /** Moyenne de lettres par mots **/
    /** 1 = rien / 2 = rien / 3 = rien **/
    evaluerFeature(m_avgSizeWord, AVG_S_WORD, "AVG_S_WORD", foundFeatures, featuresDico, 0); //0

    /** nb de mots finissant par ING **/
    /** 1 = mieux ++ / 2 = mieux + / 3 = pire **/
    evaluerFeature(m_nbFinishING, AVG_WEND_ING, "AVG_WEND_ING", foundFeatures, featuresDico, 1); //3

    /*** nb de mots commençant par une majuscule ***/
    /** 1 = mieux + / 2 = mieux ++ / 3 = mieux **/
    evaluerFeature(m_nbFirstCaps, AVG_FIRSTCAPS, "AVG_FIRSTCAPS", foundFeatures, featuresDico, 2); //2

    /*** nb de mots "I" (en majuscule) ***/
    /** 1 = pire / 2 = mieux(rien) / 3 = mieux(quasi rien) **/
    evaluerFeature(m_nbI, AVG_I_CAPS, "AVG_I_CAPS", foundFeatures, featuresDico, 0); //2

    /*** nb de mots "i" (en minuscule) ***/
    /** 1 = pire ++ / 2 = pire ++ / 3 = rien **/
    evaluerFeature(m_nbi, AVG_I_LOW, "AVG_I_LOW", foundFeatures, featuresDico, 0); //2

    /*** nb de pronoms "i,you,he,she,it,we,they" (majuscule ou minuscule) ***/
    /** 1 = mieux -- / 2 = mieux / 3 = mieux - **/
    evaluerFeature(m_nbPronoms, AVG_PRONOMS, "AVG_PRONOMS", foundFeatures, featuresDico, 2); //2

    /*** nb de "the" ***/
    /** 1 = pire / 2 = pire + / 3 = pire(presque rien) **/
    evaluerFeature(m_nbThe, AVG_THE, "AVG_THE", foundFeatures, featuresDico, 0); //2

    /*** nb de mots par phrase **/
    /** 1 = mieux + / 2 = mieux ++ / 3 = mieux **/
    evaluerFeature(corpusSize/(float)m_nbSentences, AVG_S_SENTENCE, "AVG_S_SENTENCE", foundFeatures, featuresDico, 2); //0

    /*** nb de phrases (nb de points) **/
    /** 1 = mieux + / 2 = mieux ++ / 3 = mieux **/
    evaluerFeature(m_nbSentences, AVG_POINT, "AVG_POINT", foundFeatures, featuresDico, 2); //0

    /*** nb de virgules **/
    /** 1 = pire +++ / 2 = pire ++ / 3 = mieux **/
    evaluerFeature(m_nbComma, AVG_COMMA, "AVG_COMMA", foundFeatures, featuresDico, 3); //0


    /** Caractéristiques auto sur le texte **/
    for(size_t i=0; i<corpusSize-1; i++)
    {
        /*** Caractéristiques sur les mots ***/
        addIfFound(foundFeatures, "NB_W_" + wordsList[i], featuresDico);
        /*** Caractéristiques sur les paires de mots ***/
        addIfFound(foundFeatures, "NB_2W_" + wordsList[i] + "_" + wordsList[i+1], featuresDico);
    }
    /*** dernier mot ***/
    addIfFound(foundFeatures, "NB_W_" + wordsList[corpusSize-1], featuresDico);



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

    string previousWord = "";
    while (getline(ss, item, delim))
    {
        string buff = move(item);

        char first = buff[0];
        /** mots commençant par une majuscule **/
        if(isupper(first))
        {
            m_nbFirstCaps++;
            /** pronom I majuscule **/
            if(buff == "I")
            {
                m_nbI++;
                m_nbPronoms++;
            }
        }
        else
        {
            /** pronom I minuscule **/
            if(buff == "i")
            {
                m_nbi++;
                m_nbPronoms++;
            }
        }
        // Compter les points peut suffir à compter les phrases
        // Florian
        transform(buff.begin(), buff.end(), buff.begin(), ::tolower);


        /** Pronoms : you, he, she, it, we, they **/
        if(buff.compare("you")==0 || buff.compare("he")==0 || buff.compare("she")==0 || buff.compare("it")==0 || buff.compare("we")==0 || buff.compare("they")==0 )
        {
            m_nbPronoms++;
        }
        /** The **/
        else if(buff.compare("the")==0 )
        {
            m_nbThe++;
        }
        /** . (point) **/
        else if(buff.compare(".")==0 )
        {
            m_nbSentences++;
        }
        /** , (comma) **/
        else if(buff.compare(",")==0 )
        {
            m_nbComma++;
        }

        // somme peut servir à calculer le nombre de lettres dans l'essai
        // Florian
        /* C'est utile de savoir le nombre de lettre d'un texte?? Amirali */
        size_t length = buff.size();

        /** mots finissant par -ing **/
        if(length >= 3)
        {
            if(buff.substr(length-3).compare("ing") == 0)
            {
                m_nbFinishING++;
            }
        }
        /* Changement de somme en champ m_nbLetter pour avoir le nombre de lettre d'un essay
         * Amirali
         */
        m_nbLetter += length;
        Tools::addIfAbsent(dic, "NB_W_" + buff);

        if(previousWord != "")
        {
            Tools::addIfAbsent(dic, "NB_2W_" + previousWord + "_" + buff);
        }

        previousWord = buff;
        m_wordsList.push_back(move(buff));
    }
    m_avgSizeWord = m_nbLetter/(float)m_wordsList.size();
}

void Essay::addIfFound(set<int> &found, const string &feature, map<string, int>& featuresDico)
{
    map<string, int>::iterator it = featuresDico.find(feature);
    if(it!=featuresDico.end()) found.emplace(it->second);
}

/** unsigned int features **/
void Essay::evaluerFeature(const unsigned int &val, const unsigned int borne[], const string &featureName, set<int> &found, map<string, int>& featuresDico, const unsigned int &mode){
    if(mode==1 || mode==2)
    {
        if(val <= borne[0]){
            addIfFound(found, featureName + "_INF", featuresDico);
        }
        else if(val >= borne[1])
        {
            addIfFound(found, featureName + "_SUP", featuresDico);
        }
    }
    if(mode==2 || mode==3){
        if(val > borne[0] && val < borne[1])
        {
            addIfFound(found, featureName + "_BET", featuresDico);
        }
    }
}

/** float features **/
void Essay::evaluerFeature(const float &val, const float borne[], const string &featureName, set<int> &found, map<string, int>& featuresDico, const unsigned int &mode){
    if(mode==1 || mode==2)
    {
        if(val <= borne[0]){
            addIfFound(found, featureName + "_INF", featuresDico);
        }
        else if(val >= borne[1])
        {
            addIfFound(found, featureName + "_SUP", featuresDico);
        }
    }
    if(mode==2 || mode==3){
        if(val > borne[0] && val < borne[1])
        {
            addIfFound(found, featureName + "_BET", featuresDico);
        }
    }
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
    stream <<   "Langue: " << essay.m_lang <<
                "\nNiveau: " << essay.m_level <<
                "\nWord count: " << essay.m_wordsList.size() <<
                "\nSentence count: " << essay.m_nbSentences <<
                "\nNbComma: " << essay.m_nbComma <<
                "\nNbI: " << essay.m_nbI <<
                "\nNbi: " << essay.m_nbi <<
                "\nNbPronoms: " << essay.m_nbPronoms <<
                "\nNbThe: " << essay.m_nbThe;
    return stream;
}
