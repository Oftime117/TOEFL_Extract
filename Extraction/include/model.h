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
    void corrigerMatrice(float forceCorrection, size_t langMoins, size_t langPlus, std::vector<int> &foundFeatures);
    void save();
    void setOutFiles(std::string featuresIn, std::string langMatrixIn);
    Essay& getFirstEssayP() { return m_corpusList[0]; }

protected:

private:
    // Methods
    void initModel();
    void train(std::vector<Essay> &corpus);
    void saveFeaturesDictionnary();
    void saveLangMatrix();
    void loadFeaturesDictionnary();
    void loadLangDictionnary();
    void langSetToVector(std::set<std::string> langSet);

    // Member
    std::string trainPath;
    std::string featuresPath;
    std::string langMatrixPath;
    std::vector<std::string> m_languages;
    std::map<std::string, int> m_featuresDico;
    std::vector<std::vector<float>> m_langMatrix;
    std::vector<Essay> m_corpusList;
};

#endif // MODEL_H
