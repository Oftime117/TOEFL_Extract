#include "essay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>

#include "tools.h"

using namespace std;

/*** Constantes ***/

const float Essay::AVG_WORD_CORPUS[2] = {324, 373};
const float Essay::AVG_LETTER_CORPUS[2] = {1653, 1942};
const float Essay::AVG_S_SENTENCE[2] = {19, 25};
const float Essay::AVG_S_WORD[2] = {5.08, 5.23};
/*** pourcentage d'apparition dans un texte ***/
const float Essay::AVG_WEND_ING[2] = {2.46, 3.05};
const float Essay::AVG_FIRSTCAPS[2] = {4.98, 6.82};
const float Essay::AVG_I_CAPS[2] = {0.63, 1.58};
const float Essay::AVG_I_LOW[2] = {0.08, 0.32};
const float Essay::AVG_PRONOMS[2] = {4.54, 5.68};
const float Essay::AVG_THE[2] = {13, 18};
const float Essay::AVG_POINT[2] = {3.89, 5.12};
const float Essay::AVG_COMMA[2] = {2.83, 4.58};
/*** wapiti ***/
const float Essay::WAP_NN[2] = {30, 60};
const float Essay::WAP_IN[2] = {27, 51};
const float Essay::WAP_DT[2] = {18,42};
const float Essay::WAP_NNS[2] = {14, 35};
const float Essay::WAP_JJ[2] = {14, 33};
const float Essay::WAP_VB[2] = {12, 27};
const float Essay::WAP_PRP[2] = {9, 28};
const float Essay::WAP_RB[2] = {10, 25};
const float Essay::WAP_VBP[2] = {8, 21};
const float Essay::WAP_TO[2] = {6, 16};
const float Essay::WAP_CC[2] = {6, 16};
const float Essay::WAP_VBZ[2] = {4, 14};
const float Essay::WAP_MD[2] = {3, 11};
const float Essay::WAP_VBG[2] = {2, 11};
const float Essay::WAP_PRP$[2] = {1, 9};
const float Essay::WAP_VBN[2] = {1, 7};
const float Essay::WAP_VBD[2] = {0, 7};
const float Essay::WAP_JJR[2] = {0, 6};
const float Essay::WAP_NNP[2] = {0, 11};
const float Essay::WAP_WRB[2] = {0, 4};
const float Essay::WAP_CD[2] = {0, 4};
const float Essay::WAP_WDT[2] = {0, 3};
const float Essay::WAP_WP[2] = {0, 3};
const float Essay::WAP_RBR[2] = {0, 3};
const float Essay::WAP_DT_NN[2] = {8, 23};
const float Essay::WAP_IN_DT[2] = {6, 18};
const float Essay::WAP_NN_IN[2] = {5, 16};
const float Essay::WAP_TO_VB[2] = {4, 13};
const float Essay::WAP_JJ_NN[2] = {4, 13};
const float Essay::WAP_JJ_NNS[2] = {2, 11};
const float Essay::WAP_PRP_VBP[2] = {2, 11};
const float Essay::WAP_DT_JJ[2] = {2, 10};
const float Essay::WAP_IN_NN[2] = {2, 9};
const float Essay::WAP_MD_VB[2] = {2, 9};
const float Essay::WAP_IN_PRP[2] = {1, 7};
const float Essay::WAP_NNS_IN[2] = {1, 7};
const float Essay::WAP_VB_DT[2] = {1, 6};
const float Essay::WAP_DT_NNS[2] = {0, 7};

const float Essay::AVG_TO[2] = {8, 14};
const float Essay::AVG_AND[2] = {5, 9};
const float Essay::AVG_OF[2] = {5, 9};
const float Essay::AVG_IN[2] = {5, 9};
const float Essay::AVG_A[2] = {5, 9};
const float Essay::AVG_IS[2] = {4, 8};
const float Essay::AVG_THAT[2] = {4, 8};
const float Essay::AVG_PEOPLE[2] = {2, 6};
const float Essay::AVG_IT[2] = {2, 6};
const float Essay::AVG_ARE[2] = {2, 6};
const float Essay::AVG_THEY[2] = {2, 6};
const float Essay::AVG_FOR[2] = {1, 5};
const float Essay::AVG_HAVE[2] = {1, 5};
const float Essay::AVG_NOT[2] = {1, 5};
const float Essay::AVG_BE[2] = {1, 5};
const float Essay::AVG_YOU[2] = {1, 5};
const float Essay::AVG_MORE[2] = {1, 3};
const float Essay::AVG_CAN[2] = {1, 3};
const float Essay::AVG_WILL[2] = {1, 3};
const float Essay::AVG_THEIR[2] = {1, 3};
const float Essay::AVG_WITH[2] = {1, 3};
const float Essay::AVG_WE[2] = {1, 3};
const float Essay::AVG_AS[2] = {1, 3};
const float Essay::AVG_OR[2] = {1, 3};
const float Essay::AVG_ON[2] = {0.5, 2};
const float Essay::AVG_BUT[2] = {0.5, 2};
const float Essay::AVG_IF[2] = {0.5, 2};
const float Essay::AVG_LIFE[2] = {0.5, 2};
const float Essay::AVG_THAN[2] = {0.5, 2};
const float Essay::AVG_ONE[2] = {0.5, 2};
const float Essay::AVG_YOUNG[2] = {0.5, 2};
const float Essay::AVG_TIME[2] = {0.5, 2};
const float Essay::AVG_THERE[2] = {0.5, 2};
const float Essay::AVG_BY[2] = {0.5, 2};
const float Essay::AVG_SO[2] = {0.5, 2};
const float Essay::AVG_BECAUSE[2] = {0.5, 2};
const float Essay::AVG_MY[2] = {0.5, 2};
const float Essay::AVG_ABOUT[2] = {0.5, 2};
const float Essay::AVG_HE[2] = {0.5, 2};


Essay::Essay(const string& essay, map<string, int>& dic, std::set<std::string> &langDico) throw()
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
    m_nbSentences = 0.f;
    m_nbFinishING = 0.f;
    m_nbFirstCaps = 0.f;
    m_nbI = 0.f;
    m_nbi = 0.f;
    m_nbPronoms = 0.f;
    m_nbThe = 0.f;
    m_nbComma = 0.f;

    splitEssay(' ', dic, text);

    langDico.emplace(m_lang);
}

int test = 0;
Essay::Essay(const string& essay, const string& labels, const string& labelsOcc1, const string& labelsOcc2, const string& labelsOcc3, const string& wordsOcc1, const string& wordsOcc2, const string& wordsOcc3, map<string, int>& dic, std::set<std::string> &langDico) throw()
{
    test++;
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
    /** Partie labels | l'entête est la même que l'essay pour les labels **/
    string myLabels = labels.substr(firstSpace+1);
    string myLabelsOcc1 = labelsOcc1.substr(firstSpace+1);
    string myLabelsOcc2 = labelsOcc2.substr(firstSpace+1);
//    string myLabelsOcc3 = labelsOcc3.substr(firstSpace+1);

    string myWordsOcc1 = wordsOcc1.substr(firstSpace+1);
    string myWordsOcc2 = wordsOcc2.substr(firstSpace+1);
//    string myWordsOcc3 = wordsOcc3.substr(firstSpace+1);

    m_nbSentences = 0.f;
    m_nbFinishING = 0.f;
    m_nbFirstCaps = 0.f;
    m_nbI = 0.f;
    m_nbi = 0.f;
    m_nbPronoms = 0.f;
    m_nbThe = 0.f;
    m_nbComma = 0.f;


/// cout << labels << "    " << labelsOcc1 << "    " << labelsOcc2 << "    " << labelsOcc3 << endl;
    splitEssay(' ', dic, text);
    splitLabels(' ', dic, myLabels);
    splitLabelsOcc1(' ', dic, myLabelsOcc1);
    splitLabelsOcc2(' ', dic, myLabelsOcc2);
//    splitLabelsOcc3(' ', dic, myLabelsOcc3);

    splitWordsOcc1(' ', dic, myWordsOcc1);
    splitWordsOcc2(' ', dic, myWordsOcc2);
//    splitWordsOcc3(' ', dic, myWordsOcc3);

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
    m_nbFirstCaps = other.m_nbFirstCaps;
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


    /*** caractéristiques issus de wapiti **/
    /*** nb de NN **/
    evaluerFeature(m_labelsMap["NN"], WAP_NN, "WAP_NN", foundFeatures, featuresDico, 2);

    /*** nb de IN **/
    evaluerFeature(m_labelsMap["IN"], WAP_IN, "WAP_IN", foundFeatures, featuresDico, 2);

    /*** nb de DT **/
    evaluerFeature(m_labelsMap["DT"], WAP_DT, "WAP_DT", foundFeatures, featuresDico, 2);

    /*** nb de NNS **/
    evaluerFeature(m_labelsMap["NNS"], WAP_NNS, "WAP_NNS", foundFeatures, featuresDico, 2);

    /*** nb de JJ **/
    evaluerFeature(m_labelsMap["JJ"], WAP_JJ, "WAP_JJ", foundFeatures, featuresDico, 2);

    /*** nb de VB **/
    evaluerFeature(m_labelsMap["VB"], WAP_VB, "WAP_VB", foundFeatures, featuresDico, 2);

    /*** nb de PRP **/
    evaluerFeature(m_labelsMap["PRP"], WAP_PRP, "WAP_PRP", foundFeatures, featuresDico, 2);

    /*** nb de RB **/
    evaluerFeature(m_labelsMap["RB"], WAP_RB, "WAP_RB", foundFeatures, featuresDico, 2);

    /*** nb de VBP **/
    evaluerFeature(m_labelsMap["VBP"], WAP_VBP, "WAP_VBP", foundFeatures, featuresDico, 2);

    /*** nb de TO **/
    evaluerFeature(m_labelsMap["TO"], WAP_TO, "WAP_TO", foundFeatures, featuresDico, 2);

    /*** nb de CC **/
    evaluerFeature(m_labelsMap["CC"], WAP_CC, "WAP_CC", foundFeatures, featuresDico, 2);

    /*** nb de VBZ **/
    evaluerFeature(m_labelsMap["VBZ"], WAP_VBZ, "WAP_VBZ", foundFeatures, featuresDico, 2);

    /*** nb de MD **/
    evaluerFeature(m_labelsMap["MD"], WAP_MD, "WAP_MD", foundFeatures, featuresDico, 2);

    /*** nb de VBG **/
    evaluerFeature(m_labelsMap["VBG"], WAP_VBG, "WAP_VBG", foundFeatures, featuresDico, 2);

    /*** nb de PRP$ **/
    evaluerFeature(m_labelsMap["PRP$"], WAP_PRP$, "WAP_PRP$", foundFeatures, featuresDico, 2);

    /*** nb de VBN **/
    evaluerFeature(m_labelsMap["VBN"], WAP_VBN, "WAP_VBN", foundFeatures, featuresDico, 2);

    /*** nb de VBD **/
    evaluerFeature(m_labelsMap["VBD"], WAP_VBD, "WAP_VBD", foundFeatures, featuresDico, 2);

    /*** nb de JJR **/
    evaluerFeature(m_labelsMap["JJR"], WAP_JJR, "WAP_JJR", foundFeatures, featuresDico, 2);

    /*** nb de NNP **/
    evaluerFeature(m_labelsMap["NNP"], WAP_NNP, "WAP_NNP", foundFeatures, featuresDico, 2);

    /*** nb de WRB **/
    evaluerFeature(m_labelsMap["WRB"], WAP_WRB, "WAP_WRB", foundFeatures, featuresDico, 2);

    /*** nb de CD **/
    evaluerFeature(m_labelsMap["CD"], WAP_CD, "WAP_CD", foundFeatures, featuresDico, 2);

    /*** nb de WDT **/
    evaluerFeature(m_labelsMap["WDT"], WAP_WDT, "WAP_WDT", foundFeatures, featuresDico, 2);

    /*** nb de WP **/
    evaluerFeature(m_labelsMap["WP"], WAP_WP, "WAP_WP", foundFeatures, featuresDico, 2);

    /*** nb de RBR **/
    evaluerFeature(m_labelsMap["RBR"], WAP_RBR, "WAP_RBR", foundFeatures, featuresDico, 2);
/*** paires de wapiti ***/
    /*** nb de DT_NN **/
    evaluerFeature(m_labelsMap["DT_NN"], WAP_DT_NN, "WAP_DT_NN", foundFeatures, featuresDico, 2);

    /*** nb de IN_DT **/
    evaluerFeature(m_labelsMap["IN_DT"], WAP_IN_DT, "WAP_IN_DT", foundFeatures, featuresDico, 2);

    /*** nb de NN_IN **/
    evaluerFeature(m_labelsMap["NN_IN"], WAP_NN_IN, "WAP_NN_IN", foundFeatures, featuresDico, 2);

    /*** nb de TO_VB **/
    evaluerFeature(m_labelsMap["TO_VB"], WAP_TO_VB, "WAP_TO_VB", foundFeatures, featuresDico, 2);

    /*** nb de JJ_NN **/
    evaluerFeature(m_labelsMap["JJ_NN"], WAP_JJ_NN, "WAP_JJ_NN", foundFeatures, featuresDico, 2);

    /*** nb de JJ_NNS **/
    evaluerFeature(m_labelsMap["JJ_NNS"], WAP_JJ_NNS, "WAP_JJ_NNS", foundFeatures, featuresDico, 2);

    /*** nb de PRP_VBP **/
    evaluerFeature(m_labelsMap["PRP_VBP"], WAP_PRP_VBP, "WAP_PRP_VBP", foundFeatures, featuresDico, 2);

    /*** nb de DT_JJ **/
    evaluerFeature(m_labelsMap["DT_JJ"], WAP_DT_JJ, "WAP_DT_JJ", foundFeatures, featuresDico, 2);

    /*** nb de IN_NN **/
    evaluerFeature(m_labelsMap["IN_NN"], WAP_IN_NN, "WAP_IN_NN", foundFeatures, featuresDico, 2);

    /*** nb de MD_VB **/
    evaluerFeature(m_labelsMap["MD_VB"], WAP_MD_VB, "WAP_MD_VB", foundFeatures, featuresDico, 2);

    /*** nb de IN_PRP **/
    evaluerFeature(m_labelsMap["IN_PRP"], WAP_IN_PRP, "WAP_IN_PRP", foundFeatures, featuresDico, 2);

    /*** nb de NNS_IN **/
    evaluerFeature(m_labelsMap["NNS_IN"], WAP_NNS_IN, "WAP_NNS_IN", foundFeatures, featuresDico, 2);

    /*** nb de VB_DT **/
    evaluerFeature(m_labelsMap["VB_DT"], WAP_VB_DT, "WAP_VB_DT", foundFeatures, featuresDico, 2);

    /*** nb de DT_NNS **/
    evaluerFeature(m_labelsMap["DT_NNS"], WAP_DT_NNS, "WAP_DT_NNS", foundFeatures, featuresDico, 2);


    /*** nouveaux mots à tester ***/
    /*
    evaluerFeature(m_nbTO, AVG_TO, "AVG_TO", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbAND, AVG_AND, "AVG_AND", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbOF, AVG_OF, "AVG_OF", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbIN, AVG_IN, "AVG_IN", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbA, AVG_A, "AVG_A", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbIS, AVG_IS, "AVG_IS", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbTHAT, AVG_THAT, "AVG_THAT", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbPEOPLE, AVG_PEOPLE, "AVG_PEOPLE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbIT, AVG_IT, "AVG_IT", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbARE, AVG_ARE, "AVG_ARE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbTHEY, AVG_THEY, "AVG_THEY", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbFOR, AVG_FOR, "AVG_FOR", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbHAVE, AVG_HAVE, "AVG_HAVE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbNOT, AVG_NOT, "AVG_NOT", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbBE, AVG_BE, "AVG_BE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbYOU, AVG_YOU, "AVG_YOU", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbMORE, AVG_MORE, "AVG_MORE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbCAN, AVG_CAN, "AVG_CAN", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbWILL, AVG_WILL, "AVG_WILL", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbTHEIR, AVG_THEIR, "AVG_THEIR", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbWE, AVG_WE, "AVG_WE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbAS, AVG_AS, "AVG_AS", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbOR, AVG_OR, "AVG_OR", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbON, AVG_ON, "AVG_ON", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbBUT, AVG_BUT, "AVG_BUT", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbIF, AVG_IF, "AVG_IF", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbLIFE, AVG_LIFE, "AVG_LIFE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbTHAN, AVG_THAN, "AVG_THAN", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbONE, AVG_ONE, "AVG_ONE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbYOUNG, AVG_YOUNG, "AVG_YOUNG", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbTIME, AVG_TIME, "AVG_TIME", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbBY, AVG_BY, "AVG_BY", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbTHERE, AVG_THERE, "AVG_THERE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbSO, AVG_SO, "AVG_SO", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbBECAUSE, AVG_BECAUSE, "AVG_BECAUSE", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbMY, AVG_MY, "AVG_MY", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbABOUT, AVG_ABOUT, "AVG_ABOUT", foundFeatures, featuresDico, 3);
    evaluerFeature(m_nbHE, AVG_HE, "AVG_HE", foundFeatures, featuresDico, 3);
    */

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

    for(size_t i=0; i<m_labelsList.size(); i++){
        /*** Caractéristiques sur les mots wapiti ***/
        addIfFound(foundFeatures, "NB_L_" + m_labelsList[i], featuresDico);
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
    float nbMots = m_wordsList.size();
    m_avgSizeWord = m_nbLetter/nbMots;

    m_nbFinishING /= nbMots;
    m_nbFirstCaps /= nbMots;
    m_nbI /= nbMots;
    m_nbi /= nbMots;
    m_nbPronoms /= nbMots;
    m_nbThe /= nbMots;
    m_nbSentences /= nbMots;
    m_nbComma /= nbMots;
}

void Essay::addIfFound(set<int> &found, const string &feature, map<string, int>& featuresDico)
{
    map<string, int>::iterator it = featuresDico.find(feature);
    if(it!=featuresDico.end()) found.emplace(it->second);
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

/** Stocker les labels dans leur liste **/
void Essay::splitLabels(const char& delim, map<string, int>& dic, const string& text)
{
    stringstream ss;
    ss.str(text);
    string item;
    while (getline(ss, item, delim))
    {
        string buff = item;
        Tools::addIfAbsent(dic, "NB_L_" + buff);
        m_labelsList.push_back(buff);
    }
}

/** Stocker les occurences d'un label dans sa map **/
void Essay::splitLabelsOcc1(const char& delim, map<string, int>& dic, const string& text)
{
    stringstream ss;
    ss.str(text);
    string occurence;
    string item;
    while (getline(ss, occurence, delim) && getline(ss, item, delim))
    {
        stringstream s_occurence; s_occurence.str(occurence);
        int buff1; s_occurence >> buff1;
        string buff2 = item;
        Tools::addIfAbsent(dic, "ORDRE_L1_" + buff2);
        m_labelsMap.emplace(buff2, buff1);
    }
}

/** Stocker les occurences de deux labels dans leur map **/
void Essay::splitLabelsOcc2(const char& delim, map<string, int>& dic, const string& text)
{
    stringstream ss;
    ss.str(text);
    string occurence;
    string item1, item2;
    while (getline(ss, occurence, delim) && getline(ss, item1, delim) && getline(ss, item2, delim))
    {
        stringstream s_occurence; s_occurence.str(occurence);
        int buff1; s_occurence >> buff1;
        string buff2 = item1;
        string buff3 = item2;
        Tools::addIfAbsent(dic, "ORDRE_L2_" + buff2 + "_" + buff3);
        m_labelsMap.emplace(buff2 + "_" + buff3, buff1);
    }
}

/** Stocker les occurences de trois labels dans leur map **/
void Essay::splitLabelsOcc3(const char& delim, map<string, int>& dic, const string& text)
{
    stringstream ss;
    ss.str(text);
    string occurence;
    string item1, item2, item3;
    while (getline(ss, occurence, delim) && getline(ss, item1, delim) && getline(ss, item2, delim) && getline(ss, item3, delim))
    {
        stringstream s_occurence; s_occurence.str(occurence);
        int buff1; s_occurence >> buff1;
        string buff2 = item1;
        string buff3 = item2;
        string buff4 = item3;
        Tools::addIfAbsent(dic, "ORDRE_L3_" + buff2 + "_" + buff3 + "_" + buff4);
        m_labelsMap.emplace(buff2 + "_" + buff3 + "_" + buff4, buff1);
    }
}

/** Stocker les occurences d'un mot dans sa map **/
void Essay::splitWordsOcc1(const char& delim, map<string, int>& dic, const string& text)
{
    stringstream ss;
    ss.str(text);
    string occurence;
    string item;
    while (getline(ss, occurence, delim) && getline(ss, item, delim))
    {
        stringstream s_occurence; s_occurence.str(occurence);
        int buff1; s_occurence >> buff1;
        string buff2 = item;
        Tools::addIfAbsent(dic, "ORDRE_W1_" + buff2);
        m_wordsMap.emplace(buff2, buff1);
    }
}

/** Stocker les occurences de deux mots dans leur map **/
void Essay::splitWordsOcc2(const char& delim, map<string, int>& dic, const string& text)
{
    stringstream ss;
    ss.str(text);
    string occurence;
    string item1, item2;
    while (getline(ss, occurence, delim) && getline(ss, item1, delim) && getline(ss, item2, delim))
    {
        stringstream s_occurence; s_occurence.str(occurence);
        int buff1; s_occurence >> buff1;
        string buff2 = item1;
        string buff3 = item2;
        Tools::addIfAbsent(dic, "ORDRE_W2_" + buff2 + "_" + buff3);
        m_wordsMap.emplace(buff2 + "_" + buff3, buff1);
    }
}

/** Stocker les occurences de trois mots dans leur map **/
void Essay::splitWordsOcc3(const char& delim, map<string, int>& dic, const string& text)
{
    stringstream ss;
    ss.str(text);
    string occurence;
    string item1, item2, item3;
    while (getline(ss, occurence, delim) && getline(ss, item1, delim) && getline(ss, item2, delim) && getline(ss, item3, delim))
    {
        stringstream s_occurence; s_occurence.str(occurence);
        int buff1; s_occurence >> buff1;
        string buff2 = item1;
        string buff3 = item2;
        string buff4 = item3;
        Tools::addIfAbsent(dic, "ORDRE_W3_" + buff2 + "_" + buff3 + "_" + buff4);
        m_wordsMap.emplace(buff2 + "_" + buff3 + "_" + buff4, buff1);
    }
}

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
