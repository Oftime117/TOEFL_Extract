#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

#include "essay.h"

using namespace std;

const int nbLangues = 11;

/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///
///                                                                     ///
///           PERSONNALISER LE CHEMIN VERS LE REPERTOIRE DATA           ///
///                                                                     ///
/// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///

const string dataPath = "C:/Users/Vincent/Desktop/Polytech/Et5/EIT/data/";


const string trainPath = dataPath + "train.txt";
const string caracteristiquePath = dataPath + "caracteristique.txt";
const string modelePath = dataPath + "modele.txt";

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

int main()
{
    ifstream file(trainPath, ios::in);

    if(!file)
    {
       cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    else
    {
        string line;
        //vector<Essay> essayList = vector<Essay>();
        map<string, int> dico;
        unordered_set<string> test;

        cout << trainPath << endl;
        cout << ">> Lecture du fichier" << endl << endl;

        while (std::getline(file, line))
        {
            //getline(file, line);

            Essay e(line, dico);
            //essayList.push_back(e);
        }
        file.close();

        cout << caracteristiquePath << endl;
        cout << ">> Copie de " << dico.size() << " caracteristique" << (dico.size()>=2 ? "s" : "") << endl << endl;
        cout << modelePath << endl;
        cout << ">> Initialisation d'une matrice [ " << dico.size() << " * " << nbLangues << " ]" << endl << endl;

        ofstream cFile(caracteristiquePath, ios::out);
        ofstream mFile(modelePath, ios::out);
        for(auto &a: dico)
        {
            //ecrire chaque mot unique sous la forme d'une caractéristique
            cFile << a.first << endl;
            //initialiser la ligne de la matrice pour chaque langue
            for(int i=0; i<nbLangues; i++){
                mFile << 0 << "\t";
            }
            mFile << endl;
        }
        cFile.close();
        mFile.close();
    }

    return 0;
}


