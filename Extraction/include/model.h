#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <set>
#include <vector>


class Model
{
public:
    Model(std::string corpusPath, std::string featuresPath = "data/features.feat", std::string langMatrixPath = "data/model.mtx" );
    virtual ~Model();

    void saveFeaturesDictionnary(std::string featuresPath);
    void saveLangMatrix(std::string langMatrixPath);

    void loadFeaturesDictionnary(std::string featuresPath);

protected:

private:
    // Methods
    void initModel(std::string corpusPath);

    // Const member
    const std::string dataPath = "data/";
    const std::string trainPath = dataPath + "train.txt";
    const std::string caracteristiquePath = dataPath + "caracteristique.txt";
    const std::string modelePath = dataPath + "modele.txt";

    // Member
    std::set<std::string> m_langSet;
    std::map<std::string, int> m_featuresDico;
    std::vector< std::vector<float> > m_langMatrix;
};

#endif // MODEL_H
