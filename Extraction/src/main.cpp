#include <string>
#include <sstream>
#include <iostream>
#include <time.h> //calculer le temps d'execution
#include <iostream>
#include <fstream>

#include "essay.h"
#include "model.h"

using namespace std;


int main()
{
    clock_t tStart = clock();


    //Entraînement en partant de rien

    Model modele("data/train.txt", "data/features.txt", "data/model.txt");
    modele.trainByDiv(10);

/*
    //Estimer la qualité de l'entraînement
    Model modele2("data/train.txt", "data/features.txt", "data/model.txt");
    modele2.trainByDiv(10);
    modele2.save();
*/
/*
    //Test d'une solution en se servant d'un modèle entraîné
    Model m("data/features.txt", "data/model.txt");
    m.evaluer(unEssai); //un essai non corrompu
    m.setOutFiles("data/features_2.txt", "data/model_2.txt");
    m.save();
*/

    ofstream outFile("data/features_train.txt", ios::out | ios::app);
    if(!outFile){
        //throw une exception
        return -1;
    }
    double sec = (double)(clock() - tStart)/CLOCKS_PER_SEC;
    outFile<<"Temps d'execution = "<<(int)(sec/60)<<":"<<(int)sec%60 + sec-(int)sec<<endl<<endl;
    outFile.close();

    return 0;
}


