#include "essay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

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
    m_text = essay.substr(firstSpace+1); // texte sans langue et niveau
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

    splitEssay(' ', dic, m_text);
    //spitLabels(' ', dic);
    //spitLabelsOcc1(' ', dic);
    //spitLabelsOcc2(' ', dic);
    //spitLabelsOcc3(' ', dic);
    langDico.emplace(m_lang);
}

Essay::~Essay()
{
    //dtor
}

//Copy ctor
Essay::Essay(const Essay& other) :m_text(other.m_text), m_avgSizeWord(other.m_avgSizeWord), m_nbSentences(other.m_nbSentences), m_nbFinishING(other.m_nbFinishING), m_nbFirstCaps(other.m_nbFirstCaps), m_nbI(other.m_nbI), m_nbi(other.m_nbi), m_nbPronoms(other.m_nbPronoms), m_nbThe(other.m_nbThe), m_nbComma(other.m_nbComma),
m_wordsList(other.m_wordsList), m_lang(other.m_lang), m_level(other.m_level)
{

}

Essay& Essay::operator=(const Essay& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

void Essay::splitEssay(char delim, map<string, int>& dic, const string& text)
{
    istringstream ss(text);
    string item;

    int somme = 0;
    while (getline(ss, item, delim))
    {
        string buff = item;
        size_t length = buff.size();

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
        else{
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

        dic.emplace("NB_W_" + buff, dic.size());
        m_wordsList.push_back(buff);
        // somme peut servir à calculer le nombre de lettres dans l'essai
        // Florian
        /* C'est utile de savoir le nombre de lettre d'un texte?? Amirali */
        somme += length;

        /** mots finissant par -ing **/
        if(length>=3){
            if(buff.substr(length-3).compare("ing") == 0)
            {
                m_nbFinishING++;
            }
        }
    }
    m_avgSizeWord = somme/(float)m_wordsList.size();
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
