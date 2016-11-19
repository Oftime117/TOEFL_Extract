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
        Essay(const std::string& essay, const std::string& labels, const std::string& labelsOcc1, const std::string& labelsOcc2, const std::string& labelsOcc3, const std::string& wordsOcc1, const std::string& wordsOcc2, const std::string& wordsOcc3, std::map<std::string, int>& dic, std::set<std::string> &langDico) throw();
        Essay(const std::string& essay, std::map<std::string, int>& dic, std::set<std::string> &langDico) throw();
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
        void evaluerFeature(const float &val, const float borne[], const std::string &featureName, std::set<int> &found, std::map<std::string, int>& featuresDico, const unsigned int &mode);
        void splitLabels(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitLabelsOcc1(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitLabelsOcc2(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitLabelsOcc3(const char& delim, std::map<std::string, int>& dic, const std::string& text);

        void splitWordsOcc1(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitWordsOcc2(const char& delim, std::map<std::string, int>& dic, const std::string& text);
        void splitWordsOcc3(const char& delim, std::map<std::string, int>& dic, const std::string& text);

        /** Membres **/
        //std::string m_text;
        size_t m_textSize;
        size_t m_nbLetter;

        float m_avgSizeWord;
        unsigned int m_nbSentences;
        std::vector<std::string> m_wordsList;
        std::vector<std::string> m_labelsList;
        std::map<std::string, int> m_wordsMap;
        std::map<std::string, int> m_labelsMap;
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
        static const float AVG_WORD_CORPUS[];
        static const float AVG_LETTER_CORPUS[];
        static const float AVG_S_WORD[];
        static const float AVG_WEND_ING[];
        static const float AVG_FIRSTCAPS[];
        static const float AVG_I_CAPS[];
        static const float AVG_I_LOW[];
        static const float AVG_PRONOMS[];
        static const float AVG_THE[];
        static const float AVG_S_SENTENCE[];
        static const float AVG_POINT[];
        static const float AVG_COMMA[];
        static const float WAP_NN[];
        static const float WAP_IN[];
        static const float WAP_DT[];
        static const float WAP_NNS[];
        static const float WAP_JJ[];
        static const float WAP_VB[];
        static const float WAP_PRP[];
        static const float WAP_RB[];
        static const float WAP_VBP[];
        static const float WAP_TO[];
        static const float WAP_CC[];
        static const float WAP_VBZ[];
        static const float WAP_MD[];
        static const float WAP_VBG[];
        static const float WAP_PRP$[];
        static const float WAP_VBN[];
        static const float WAP_VBD[];
        static const float WAP_JJR[];
        static const float WAP_NNP[];
        static const float WAP_WRB[];
        static const float WAP_CD[];
        static const float WAP_WDT[];
        static const float WAP_WP[];
        static const float WAP_RBR[];
        static const float WAP_DT_NN[];
        static const float WAP_IN_DT[];
        static const float WAP_NN_IN[];
        static const float WAP_TO_VB[];
        static const float WAP_JJ_NN[];
        static const float WAP_JJ_NNS[];
        static const float WAP_PRP_VBP[];
        static const float WAP_DT_JJ[];
        static const float WAP_IN_NN[];
        static const float WAP_MD_VB[];
        static const float WAP_IN_PRP[];
        static const float WAP_NNS_IN[];
        static const float WAP_VB_DT[];
        static const float WAP_DT_NNS[];
        static const float AVG_TO[];
        static const float AVG_AND[];
        static const float AVG_OF[];
        static const float AVG_IN[];
        static const float AVG_A[];
        static const float AVG_IS[];
        static const float AVG_THAT[];
        static const float AVG_PEOPLE[];
        static const float AVG_IT[];
        static const float AVG_ARE[];
        static const float AVG_THEY[];
        static const float AVG_FOR[];
        static const float AVG_HAVE[];
        static const float AVG_NOT[];
        static const float AVG_BE[];
        static const float AVG_YOU[];
        static const float AVG_MORE[];
        static const float AVG_CAN[];
        static const float AVG_WILL[];
        static const float AVG_THEIR[];
        static const float AVG_WITH[];
        static const float AVG_WE[];
        static const float AVG_AS[];
        static const float AVG_OR[];
        static const float AVG_ON[];
        static const float AVG_BUT[];
        static const float AVG_IF[];
        static const float AVG_LIFE[];
        static const float AVG_THAN[];
        static const float AVG_ONE[];
        static const float AVG_YOUNG[];
        static const float AVG_TIME[];
        static const float AVG_THERE[];
        static const float AVG_BY[];
        static const float AVG_SO[];
        static const float AVG_BECAUSE[];
        static const float AVG_MY[];
        static const float AVG_ABOUT[];
        static const float AVG_HE[];
};

#endif // ESSAY_H
