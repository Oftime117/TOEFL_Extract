#ifndef ESSAY_H
#define ESSAY_H

#include <string>
#include <vector>
#include <map>
#include <set>


class Essay
{
    public:
        /** Constructeurs && Destructeur **/
        Essay(const std::string& essay, std::map<std::string, int>& dic, std::set<std::string> &langDico) throw();
        Essay(const Essay& other);
        Essay& operator=(const Essay& other);
        Essay(Essay &&rvalue);
        //virtual ~Essay();

        /** Operateurs **/
        Essay& operator=(Essay && rvalue);
        friend std::ostream& operator<< (std::ostream& stream, const Essay& essay);


        /** Méthodes **/
        int getNbSentences() const { return m_nbSentences; }
        float getSizeWord() const { return m_sizeWord; }
        const std::string& getText() const { return m_text; }
        const std::vector<std::string>& getWordsList() const { return m_wordsList; }
        const std::string& getLang() const { return m_lang; }
        const std::string& getLevel() const { return m_level; }

        size_t evaluer(const size_t& nbLang, std::map<std::string, int>& featuresDico,
        std::vector<std::vector<float> >& langMatrix, std::set<int>& foundFeatures);
        size_t evaluer(const size_t& nbLang, std::map<std::string, int>& featuresDico,
        std::vector<std::vector<float> >& langMatrix);

    protected:

    private:

        /** Méthodes **/
        void splitEssay(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void addIfFound(std::set<int> &found, const std::string &feature, std::map<std::string, int>& featuresDico);
//        void splitLabels(char delim, std::map<std::string, int>& dic);
//        void Essay::spitLabelsOcc1(char delim, std::map<std::string, int>& dic)
//        void Essay::spitLabelsOcc1(char delim, std::map<std::string, int>& dic)
//        void Essay::spitLabelsOcc1(char delim, std::map<std::string, int>& dic)

        /** Membres **/
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

        /** Const **/
        const static unsigned int SZ_CORPUS_INF;
        const static unsigned int SZ_CORPUS_SUP;
        const static unsigned int NB_LETTER_INF;
        const static unsigned int NB_LETTER_SUP;
        const static float SZ_WORD_INF;
        const static float SZ_WORD_SUP;
};

#endif // ESSAY_H
