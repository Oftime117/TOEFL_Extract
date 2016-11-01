#include "essay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

Essay::Essay(const string& text, /* const string& labels, const string& labelsOcc1, const string& labelsOcc2, const string& labelsOcc3,*/ map<string, int>& dic, std::set<std::string> &langDico)
{
    size_t firstSpace = text.find_first_of(' ', 0); // (LANGUE,NIVEAU)
    size_t firstComa = text.find_first_of(',', 0);
    size_t firstCP = text.find_first_of(')', 0);
    m_lang = text.substr(1, firstComa-1);
    m_level = text.substr(firstComa+1, firstCP-firstComa-1);
    m_text = text.substr(firstSpace+1); // texte sans langue et niveau
    //m_labels = labels.substr(firstSpace+1); // l'entête est la même
    //m_labelsOcc1 = labelsOcc1.substr(firstSpace+1);
    //m_labelsOcc2 = labelsOcc2.substr(firstSpace+1);
    //m_labelsOcc3 = labelsOcc3.substr(firstSpace+1);
    m_nbSentences = 0;

    splitEssay(' ', dic);
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
Essay::Essay(const Essay& other) :m_text(other.m_text), m_sizeWord(other.m_sizeWord),
m_wordsList(other.m_wordsList), m_lang(other.m_lang), m_level(other.m_level)
{

}

Essay& Essay::operator=(const Essay& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

void Essay::splitEssay(char delim, map<string, int>& dic)
{
    stringstream ss;
    ss.str(m_text);
    string item;
    vector<string> wordsList;
    int somme = 0;
    while (getline(ss, item, delim))
    {
        string buff = item;
        // Faudrait peut être compter le nombre de majuscules avant de tolower
        // ça peut être une caractéristiques
        // Amirali
        // Compter les points peut suffir à compter les phrases
        // Florian
        transform(buff.begin(), buff.end(), buff.begin(), ::tolower);
        dic.emplace("NB_W_" + buff, dic.size());
        wordsList.push_back(buff);
        if (buff.compare(".") == 0) m_nbSentences++;
        // somme peut servir à calculer le nombre de lettres dans l'essai
        // Florian
        somme += buff.size();
    }
    m_wordsList = wordsList;
    m_sizeWord = somme/(float)m_wordsList.size();
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

std::ostream& operator<< (std::ostream& stream, const Essay& essay)
{
    stream << "Langue: " << essay.m_lang << "\nNiveau: " << essay.m_level << "\nWord count: " << essay.m_wordsList.size() << "\nSentence count: " << essay.m_nbSentences;
    return stream;
}
