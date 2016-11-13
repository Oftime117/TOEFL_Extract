#ifndef CORPUS_H
#define CORPUS_H

#include <vector>
#include <map>
#include <string>

#include "essay.h"

class Corpus
{
    public:
        /** Constructeurs && Destructeur **/
        Corpus(const std::vector<std::string>& languages,
               std::map<std::string, int>& featuresDico,
               std::vector<Essay>& corpusList, const size_t& nbDiv, const size_t& numTrain);
        Corpus(const std::vector<std::string>& languages, std::map<std::string, int>& featuresDico,
               std::vector<Essay>& testCorpusList);
        Corpus(const Corpus& other);
        //virtual ~Corpus();
        //Corpus& operator=(const Corpus& other);

        /** Méthodes **/
        float train(bool verbose = false);

        void resetConfusionMatrix();
        std::vector<std::vector<int>>& getConfusionMatrix(){ return m_confusionMatrix; }
        void printConfusionMatrix(const std::string &path) const;

    protected:

    private:

        /** Méthodes **/
        /* Retourner l'indice de langue trouvé ?
         * Amirali
         */
        int evaluer(std::vector<Essay>& subCorpus, const float& forceCorrection=0);
        void addIfFound(std::set<int>& found, const std::string& feature);
        void corrigerMatrice(const float& forceCorrection, const size_t& langMoins,
                             const size_t& langPlus, const std::set<int>& foundFeatures);

        /** Membres **/
        size_t m_numTrain;
        const std::vector<std::string>& m_languages;
        std::map<std::string, int>& m_featuresDico;
        std::vector<std::vector<float>> m_langMatrix;
        std::vector<std::vector<int>> m_confusionMatrix;
        std::vector<Essay> m_trainCorpus;
        std::vector<Essay> m_testCorpus;


};

#endif // CORPUS_H
