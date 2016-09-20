#ifndef ESSAY_H
#define ESSAY_H

#include <string>
#include <vector>


class Essay
{
    public:
        Essay(std::string text);
        virtual ~Essay();
        Essay(const Essay& other);
        Essay& operator=(const Essay& other);
        std::string GetText() { return m_text; }
        std::vector<std::string> GetwordsList() { return m_wordsList; }
        std::string Getlang() { return m_lang; }
        std::string getLevel() { return m_level;}

        friend std::ostream& operator<< (std::ostream& stream, const Essay& essay);
    protected:

    private:
        std::string m_text;
        std::vector<std::string> m_wordsList;
        std::string m_lang;
        std::string m_level;

        void splitEssay(const std::string &s, char delim);
};

#endif // ESSAY_H
