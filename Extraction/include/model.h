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
    Model(std::vector<Essay> &corpusList);
    virtual ~Model();

    void trainAll();
    float trainByDiv(size_t nbDiv=10);
    bool evaluer(Essay &e, float forceCorrection=0);
    void corrigerMatrice(float forceCorrection, size_t langMoins, size_t langPlus, std::set<int> &foundFeatures);
    void save();
    void setOutFiles(std::string featuresIn, std::string langMatrixIn);
    Essay& getFirstEssayP() { return m_corpusList[0]; }

protected:

private:
    // Methods
    void initModel();
    void train(std::vector<Essay> &corpus);
    void addIfFound(std::set<int> &found, const std::string &feature);
    void saveFeaturesDictionnary();
    void saveLangMatrix();
    void loadFeaturesDictionnary();
    void loadLangDictionnary();
    void langSetToVector(std::set<std::string> langSet);

    // Member
    std::string m_trainPath;
    std::string m_featuresPath;
    std::string m_langMatrixPath;
    std::vector<std::string> m_languages;
    std::map<std::string, int> m_featuresDico;
    std::vector<std::vector<float>> m_langMatrix;
    std::vector<Essay> m_corpusList;

    // const
    static unsigned int SZ_CORPUS_INF;
    static unsigned int SZ_CORPUS_SUP;
    static unsigned int NB_LETTER_INF;
    static unsigned int NB_LETTER_SUP;
    static float SZ_WORD_INF;
    static float SZ_WORD_SUP;
};

#endif // MODEL_H
