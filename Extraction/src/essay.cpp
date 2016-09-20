#include "essay.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Essay::Essay(string text)
{
    m_text = text;

    size_t found = m_text.find_first_of(' ', 0); // (LANGUE,NIVEAU)
    m_lang = m_text.substr(0, found);
    string line_cut = m_text.substr(found+1); // line cut do not contain the first word
    splitEssay(line_cut, ' ');

    //cout << GetText().size();
    //cout << *this << endl;

}

Essay::~Essay()
{
    //dtor
}

Essay::Essay(const Essay& other)
{
    m_text = other.m_text;
    m_lang = other.m_lang;
    m_level = other.m_level;
    m_wordsList = other.m_wordsList;
}

Essay& Essay::operator=(const Essay& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

void Essay::splitEssay(const string &s, char delim)
{
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim))
    {
        m_wordsList.push_back(item);
    }
}

std::ostream& operator<< (std::ostream& stream, const Essay& essay)
{
    stream << "Langue: " << essay.m_lang << "\nNiveau: " << essay.m_level << "\nWord count: " << essay.m_wordsList.size();
    return stream;
}
