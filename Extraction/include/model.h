#ifndef MODEL_H
#define MODEL_H

#include <algorithm> //random_shuffle
#include <map>
#include <set>
#include <vector>
#include "essay.h"

class Model
{
    public:
        /** Constructeurs && Destructeur **/
        Model(std::string corpusPath, std::string featuresOut, std::string langMatrixOut);
        Model(std::string featuresIn, std::string langMatrixIn);
        Model(std::vector<Essay> &corpusList);
        //virtual ~Model();

        void trainAll();
        float trainByDiv(const size_t& nbDiv=10, const size_t& stop=-1); /* un unsigned int qui vaut -1??? - Amirali */
        float trainByDiv2(const size_t& nbDiv=10);
        float trainByDiv3(const size_t& nbDiv=10);
        void save();
        void setOutFiles(std::string featuresIn, std::string langMatrixIn);
        Essay& getFirstEssay() { return m_corpusList[0]; }

        void resetConfusionMatrix();
        void printConfusionMatrix(const std::string &path) const;

        void addFeaturePerso(const std::string& f);
        void addFeatureAndResize(const std::string& f);

    protected:

    private:
        /** Méthodes **/
        void initModel();
        void train(std::vector<Essay> &corpus);
        bool evaluer(Essay &e, const float& forceCorrection=0);
        void addIfFound(std::set<int> &found, const std::string &feature);
        void corrigerMatrice(const float& forceCorrection, const size_t& langMoins, const size_t& langPlus, const std::set<int> &foundFeatures);
        void saveFeaturesDictionnary() const throw();
        void saveLangMatrix()const throw();
        void loadFeaturesDictionnary() throw();
        void loadLangDictionnary() throw();

        void evaluerFeature(const unsigned int &val, const unsigned int borne[], const std::string &featureName, std::set<int> &found, const unsigned int &mode); /** unsigned int features **/
        void evaluerFeature(const float &val, const float borne[], const std::string &featureName, std::set<int> &found, const unsigned int &mode); /** float features **/



        /** Membres **/
        std::string m_trainPath;
        std::string m_featuresPath;
        std::string m_langMatrixPath;
        std::vector<std::string> m_languages;
        std::map<std::string, int> m_featuresDico;
        std::vector<std::vector<float>> m_langMatrix;
        std::vector<std::vector<int>> m_confusionMatrix;
        std::vector<Essay> m_corpusList;

        /** Const **/
        const static bool printTrainConfusionMatrix;

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
        static const unsigned int AVG_TO[];
        static const unsigned int AVG_AND[];
        static const unsigned int AVG_OF[];
        static const unsigned int AVG_IN[];
        static const unsigned int AVG_A[];
        static const unsigned int AVG_IS[];
        static const unsigned int AVG_THAT[];
        static const unsigned int AVG_PEOPLE[];
        static const unsigned int AVG_IT[];
        static const unsigned int AVG_ARE[];
        static const unsigned int AVG_THEY[];
        static const unsigned int AVG_FOR[];
        static const unsigned int AVG_HAVE[];
        static const unsigned int AVG_NOT[];
        static const unsigned int AVG_BE[];
        static const unsigned int AVG_YOU[];
        static const unsigned int AVG_MORE[];
        static const unsigned int AVG_CAN[];
        static const unsigned int AVG_WILL[];
        static const unsigned int AVG_THEIR[];
        static const unsigned int AVG_WITH[];
        static const unsigned int AVG_WE[];
        static const unsigned int AVG_AS[];
        static const unsigned int AVG_OR[];
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

#endif // MODEL_H
