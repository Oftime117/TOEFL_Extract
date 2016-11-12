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
        const unsigned int getNbSentences() const { return m_nbSentences; }
        const float getSizeWord() const { return m_avgSizeWord; }
        const std::string& getText() const { return m_text; }
        const unsigned int getTextSize() const { return m_text.size(); }
        const std::vector<std::string>& getWordsList() { return m_wordsList; }
        const std::string getLang() const { return m_lang; }
        const std::string getLevel() const { return m_level; }
        const unsigned int getNbFinishING() const { return m_nbFinishING; }
        const unsigned int getNbFirstCaps() const { return m_nbFirstCaps; }
        const unsigned int getNbI() const { return m_nbI; }
        const unsigned int getNbi() const { return m_nbi; }
        const unsigned int getNbPronoms() const { return m_nbPronoms; }
        const unsigned int getNbThe() const { return m_nbThe; }
        const unsigned int getNbComma() const { return m_nbComma; }

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

        float m_avgSizeWord;
        unsigned int m_nbSentences;
        std::vector<std::string> m_wordsList;
        std::vector<std::string> m_labelsList;
        std::string m_lang;
        std::string m_level;
        unsigned int m_nbFinishING;
        unsigned int m_nbFirstCaps;
        unsigned int m_nbI;
        unsigned int m_nbi;
        unsigned int m_nbPronoms;
        unsigned int m_nbThe;
        unsigned int m_nbComma;
        //unordered_set& m_dictionnary;

        size_t test;
};

#endif // ESSAY_H
