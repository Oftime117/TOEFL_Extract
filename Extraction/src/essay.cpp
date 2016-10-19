#include "essay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

Essay::Essay(string text, map<string, int>& dic, std::set<std::string> &langDico)
{
    m_text = text;

    size_t firstSpace = m_text.find_first_of(' ', 0); // (LANGUE,NIVEAU)
    size_t firstComa = m_text.find_first_of(',', 0);
    size_t firstCP = m_text.find_first_of(')', 0);
    m_lang = m_text.substr(1, firstComa-1);
    m_level = m_text.substr(firstComa+1, firstCP-firstComa-1);
    string line_cut = m_text.substr(firstSpace+1); // line cut do not contain the first word
    splitEssay(line_cut, ' ', dic);

    langDico.emplace(m_lang);
}

Essay::~Essay()
{
    //dtor
}

//Copy ctor
Essay::Essay(const Essay& other) :m_text(other.m_text), m_wordsList(other.m_wordsList), m_lang(other.m_lang), m_level(other.m_level)
{

}

Essay& Essay::operator=(const Essay& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

void Essay::splitEssay(const string &s, char delim, map<string, int>& dic)
{
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim))
    {
        //to lower
        string buff = item;
        transform(buff.begin(), buff.end(), buff.begin(), ::tolower);
        dic.emplace("NB_W_" + buff, dic.size());
    }
}

std::ostream& operator<< (std::ostream& stream, const Essay& essay)
{
    stream << "Langue: " << essay.m_lang << "\nNiveau: " << essay.m_level << "\nWord count: " << essay.m_wordsList.size();
    return stream;
}
