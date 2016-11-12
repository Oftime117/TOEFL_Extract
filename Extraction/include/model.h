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
    Model(std::string corpusPath, std::string featuresOut, std::string langMatrixOut);
    Model(std::string featuresIn, std::string langMatrixIn);
    virtual ~Model();

    void trainAll();
    float trainByDiv(const size_t& nbDiv=10, const size_t& stop=-1);
    bool evaluer(Essay &e, const float& forceCorrection=0);
    void corrigerMatrice(float forceCorrection, size_t langMoins, size_t langPlus, std::set<int> &foundFeatures);
    void save();
    void setOutFiles(std::string featuresIn, std::string langMatrixIn);
    Essay& getFirstEssay() { return m_corpusList[0]; }
    void resetConfusionMatrix();
    void printConfusionMatrix(const std::string &path) const;

protected:

private:
    /** Methods **/
    void initModel();
    void train(std::vector<Essay> &corpus);
    void addFeaturePerso(const std::string &f);
    void addFeatureAndResize(const std::string &f);
    void addIfFound(std::set<int> &found, const std::string &feature);
    void saveFeaturesDictionnary() const throw();
    void saveLangMatrix()const throw();
    void loadFeaturesDictionnary() throw();
    void loadLangDictionnary() throw();
    void evaluerFeature(const unsigned int &val, const unsigned int borne[], const std::string &featureName, std::set<int> &found, const unsigned int &mode); /** unsigned int features **/
    void evaluerFeature(const float &val, const float borne[], const std::string &featureName, std::set<int> &found, const unsigned int &mode); /** float features **/


    /** Members **/
    std::string m_trainPath;
    std::string m_featuresPath;
    std::string m_langMatrixPath;
    std::vector<std::string> m_languages;
    std::map<std::string, int> m_featuresDico;
    std::vector<std::vector<float>> m_langMatrix;
    std::vector<std::vector<int>> m_confusionMatrix;
    std::vector<Essay> m_corpusList;

    /** Const **/
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

#endif // MODEL_H
