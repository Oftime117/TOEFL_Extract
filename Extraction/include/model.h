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

        void trainAll();
        float trainByDiv2(const size_t& nbDiv=10);
        float trainByDiv3(const size_t& nbDiv=10);
        void save();
        void setOutFiles(std::string featuresIn, std::string langMatrixIn);
        //Essay& getFirstEssay() { return m_corpusList[0]; }

        void resetConfusionMatrix();
        void printConfusionMatrix(const std::string &path) const;

        void addFeaturePerso(const std::string& f);

    protected:

    private:
        /** Méthodes **/
        void initModel();
        void corrigerMatrice(const float& forceCorrection, const size_t& langMoins, const size_t& langPlus, const std::set<int> &foundFeatures);
        void saveFeaturesDictionnary() const throw();
        void saveLangMatrix()const throw();
        void loadFeaturesDictionnary() throw();
        void loadLangDictionnary() throw();




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
};

#endif // MODEL_H
