#ifndef ESSAY_H
#define ESSAY_H

#include <string>
#include <vector>
#include <map>
#include <set>


class Essay
{
    public:
        Essay(std::string text, std::map<std::string, int>& dic, std::set<std::string> &langDico);
        virtual ~Essay();
        Essay(const Essay& other);
        Essay& operator=(const Essay& other);
        std::string getText() { return m_text; }
        std::vector<std::string> getWordsList() { return m_wordsList; }
        std::string getLang() { return m_lang; }
        std::string getLevel() { return m_level;}

        friend std::ostream& operator<< (std::ostream& stream, const Essay& essay);
    protected:

    private:
        std::string m_text;
        std::vector<std::string> m_wordsList;
        std::string m_lang;
        std::string m_level;
        //unordered_set& m_dictionnary;

        void splitEssay(const std::string &s, char delim, std::map<std::string, int>& dic);
};

#endif // ESSAY_H
