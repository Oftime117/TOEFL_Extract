#ifndef ESSAY_H
#define ESSAY_H

#include <string>
#include <vector>
#include <map>
#include <set>


class Essay
{
    public:
        Essay(const std::string& text, std::map<std::string, int>& dic, std::set<std::string> &langDico);
        virtual ~Essay();
        Essay(const Essay& other);
        Essay& operator=(const Essay& other);
        float getSizeWord() const { return m_sizeWord; }
        const std::string& getText() const { return m_text; }
        const std::vector<std::string>& getWordsList() { return m_wordsList; }
        std::string getLang() const { return m_lang; }
        std::string getLevel() const { return m_level;}

        friend std::ostream& operator<< (std::ostream& stream, const Essay& essay);
    protected:

    private:
        std::string m_text;
        // Je comprends pas trop ce que c'est
        // La longueur moyenne des mots de l'essai?
        // Peut être à renommer du coup? :)
        // Amirali
        float m_sizeWord;
        std::vector<std::string> m_wordsList;
        std::string m_lang;
        std::string m_level;
        //unordered_set& m_dictionnary;

        void splitEssay(char delim, std::map<std::string, int>& dic);
        void splitLabels(char delim, map<string, int>& dic);
        size_t test;
};

#endif // ESSAY_H
