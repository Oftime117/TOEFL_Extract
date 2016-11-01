#ifndef ESSAY_H
#define ESSAY_H

#include <string>
#include <vector>
#include <map>
#include <set>


class Essay
{
    public:
        Essay(const std::string& essay, std::map<std::string, int>& dic, std::set<std::string> &langDico) throw();
        virtual ~Essay();
        Essay(const Essay& other);
        Essay& operator=(const Essay& other);
        int getNbSentences() const { return m_nbSentences; }
        float getSizeWord() const { return m_sizeWord; }
        const std::string& getText() const { return m_text; }
        const std::vector<std::string>& getWordsList() { return m_wordsList; }
        std::string getLang() const { return m_lang; }
        std::string getLevel() const { return m_level; }

        friend std::ostream& operator<< (std::ostream& stream, const Essay& essay);
    protected:

    private:

        /** Methods **/
        void splitEssay(char delim, std::map<std::string, int>& dic, const std::string& text);
//        void splitLabels(char delim, std::map<std::string, int>& dic);
//        void Essay::spitLabelsOcc1(char delim, std::map<std::string, int>& dic)
//        void Essay::spitLabelsOcc1(char delim, std::map<std::string, int>& dic)
//        void Essay::spitLabelsOcc1(char delim, std::map<std::string, int>& dic)

        /** Members **/
        std::string m_text;

        /* Je comprends pas trop ce que c'est
         * La longueur moyenne des mots de l'essai?
         * Peut être à renommer du coup? :)
         * Amirali
         */
        float m_sizeWord;
        int m_nbSentences;
        std::vector<std::string> m_wordsList;
        std::vector<std::string> m_labelsList;
        std::string m_lang;
        std::string m_level;
        //unordered_set& m_dictionnary;

        size_t test;
};

#endif // ESSAY_H
