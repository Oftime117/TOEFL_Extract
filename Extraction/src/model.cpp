#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "model.h"
#include "essay.h"

using namespace std;

Model::Model(string corpusPath, string featuresPath, string langMatrixPath)
    : m_langSet(), m_featuresDico(), m_langMatrix()
{
    ifstream featuresFile(featuresPath, ios::in);
    ifstream langMatrixFile(langMatrixPath, ios::in);

    if(!featuresFile || !langMatrixFile)
    {
        // Création du modèle
        initModel(corpusPath);
    }
    saveFeaturesDictionnary(featuresPath);
    saveLangMatrix(langMatrixPath);
}

Model::~Model()
{
    //dtor
}

void Model::initModel(string corpusPath)
{
    ifstream corpusFile(corpusPath, ios::in);
    if(!corpusFile)
    {
       cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    else
    {
        string line;
        cout << corpusPath << endl;
        cout << ">> Lecture du fichier" << endl << endl;

        // Lecture du corpus et enregistrement des caractéristiques (juste les mots for now))
        while (getline(corpusFile, line))
        {
            Essay(line, m_featuresDico, m_langSet);
        }
        corpusFile.close();

        cout << caracteristiquePath << endl;
        cout << ">> Copie de " << m_featuresDico.size() << " caracteristiques" << endl << endl;
        cout << modelePath << endl;
        cout << ">> Initialisation d'une matrice [ " << m_featuresDico.size() << " * " << m_langSet.size() << " ]" << endl << endl;

        // Initialisation de la matrice avec des 0
        m_langMatrix.resize(m_featuresDico.size(), vector<float>(m_langSet.size()));
    }
}

void Model::saveFeaturesDictionnary(string featuresPath)
{
    cout << ">> Sauvegarde des caracteristiques\n";

    ofstream cFile(featuresPath, ios::out);

    if(!cFile)
    {
        //throw une exception
        return;
    }
    cFile << "NB_FEATURES: " << m_featuresDico.size() << endl;
    for(auto &a: m_featuresDico)
    {
        //ecrire chaque mot unique sous la forme d'une caractéristique
        cFile << a.first << endl;
    }
    cFile.close();
}

void Model::saveLangMatrix(string langMatrixPath)
{
    cout << ">> Sauvegarde de la matrice des langues\n";
    ofstream mFile(langMatrixPath, ios::out);

    if(!mFile)
    {
        //throw une exception
        return;
    }

    // Ajout des headers des langues
    set<string>::iterator langSet_it = m_langSet.begin();
    while(langSet_it != m_langSet.end())
    {
        mFile << *langSet_it;
        langSet_it++;
        if(langSet_it != m_langSet.end())
            mFile << "\t";
    }
    mFile << endl;

    for(size_t i = 0; i < m_featuresDico.size(); ++i)
    {
        for(size_t j = 0; j < m_langSet.size(); ++j)
        {
            mFile << m_langMatrix[i][j];
            if(j < m_langSet.size()-1)
                mFile << "\t";
        }
        if(i < m_featuresDico.size() - 1) mFile << endl;
    }
    mFile.close();
}

void Model::loadFeaturesDictionnary(string featuresPath)
{
   istream cFile(featuresPath, ios::in)
   if(!cFile)
   {
       //throw exception
       return;
   }

   //TODO
}
