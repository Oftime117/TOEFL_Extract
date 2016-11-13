#ifndef ESSAY_H
#define ESSAY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>


class Essay
{
    public:
        /** Constructeurs && Destructeur **/
        Essay(const std::string& essay, const std::string& labels, const std::string& labelsOcc1, const std::string& labelsOcc2, const std::string& labelsOcc3, std::map<std::string, int>& dic, std::set<std::string> &langDico) throw();
        //Essay(const std::string& essay, std::map<std::string, int>& dic, std::set<std::string> &langDico) throw();
        Essay(const Essay& other);
        Essay(Essay &&rvalue);
        //virtual ~Essay();

        /** Operateurs **/
        Essay& operator=(const Essay& other);
        Essay& operator=(Essay && rvalue);
        friend std::ostream& operator<< (std::ostream& stream, const Essay& essay);


        /** Méthodes **/
        int getNbSentences() const { return m_nbSentences; }
        float getSizeWord() const { return m_avgSizeWord; }
        //const std::string& getText() const { return m_text; }
        const std::vector<std::string>& getWordsList() const { return m_wordsList; }
        const std::string& getLang() const { return m_lang; }
        const std::string& getLevel() const { return m_level; }
        const unsigned int& getTextSize() const { return m_textSize; }
        const unsigned int& getNbFinishING() const { return m_nbFinishING; }
        const unsigned int& getNbFirstCaps() const { return m_nbFirstCaps; }
        const unsigned int& getNbI() const { return m_nbI; }
        const unsigned int& getNbi() const { return m_nbi; }
        const unsigned int& getNbPronoms() const { return m_nbPronoms; }
        const unsigned int& getNbThe() const { return m_nbThe; }
        const unsigned int& getNbComma() const { return m_nbComma; }

        size_t evaluer(const size_t& nbLang, std::map<std::string, int>& featuresDico,
        std::vector<std::vector<float> >& langMatrix, std::set<int>& foundFeatures);
        size_t evaluer(const size_t& nbLang, std::map<std::string, int>& featuresDico,
        std::vector<std::vector<float> >& langMatrix);


    protected:

    private:

        /** Méthodes **/
        void splitEssay(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void addIfFound(std::set<int> &found, const std::string &feature, std::map<std::string, int>& featuresDico);

        void evaluerFeature(const unsigned int &val, const unsigned int borne[], const std::string &featureName, std::set<int> &found, std::map<std::string, int>& featuresDico, const unsigned int &mode); /** unsigned int features **/
        void evaluerFeature(const float &val, const float borne[], const std::string &featureName, std::set<int> &found, std::map<std::string, int>& featuresDico, const unsigned int &mode); /** float features **/

        void splitLabels(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitLabelsOcc1(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitLabelsOcc2(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitLabelsOcc3(const char& delim, std::map<std::string, int>& dic, const std::string& text);

        /** Membres **/
        //std::string m_text;
        size_t m_textSize;
        size_t m_nbLetter;

        float m_avgSizeWord;
        unsigned int m_nbSentences;
        std::vector<std::string> m_wordsList;
        std::vector<std::string> m_labelsList;
        std::unordered_map<std::string, int> m_labelsMap;
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

        /** Const **/
        const static unsigned int SZ_CORPUS_INF;
        const static unsigned int SZ_CORPUS_SUP;
        const static unsigned int NB_LETTER_INF;
        const static unsigned int NB_LETTER_SUP;
        const static float SZ_WORD_INF;
        const static float SZ_WORD_SUP;

        static const unsigned int AVG_WORD_CORPUS[];
        static const unsigned int AVG_LETTER_CORPUS[];
        static const float AVG_S_WORD[];
        static const unsigned int AVG_WEND_ING[];
        static const unsigned int AVG_FIRSTCAPS[];
        static const unsigned int AVG_I_CAPS[];
        static const unsigned int AVG_I_LOW[];
        static const unsigned int AVG_PRONOMS[];
        static const unsigned int AVG_THE[];
        static const unsigned int AVG_S_SENTENCE[];
        static const unsigned int AVG_POINT[];
        static const unsigned int AVG_COMMA[];
};

#endif // ESSAY_H
