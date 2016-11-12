#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "essay.h"
#include "model.h"

using namespace std;


int main(/*int argc, char** argv*/)
{
    //clock_t tStart = clock();

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    //Entraînement en partant de rien

    Model modele("data/train.txt", "data/features.txt", "data/model.txt");

    end = chrono::system_clock::now();

    auto loadElapsedMilli = chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    cout << "*** Temps de chargement des caracteristiques: " << loadElapsedMilli << "ms ***"<< endl << endl;
    cout <<"nb threads " << thread::hardware_concurrency() << endl;

    modele.trainByDiv3(10);

    start = chrono::system_clock::now();
    auto CVTrainElapsedMilli = chrono::duration_cast<std::chrono::milliseconds>
                             (start-end).count();
    cout << "*** Temps de l'entrainement: " << CVTrainElapsedMilli << "ms ***"<< endl << endl;
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

//    ofstream outFile("data/features_train.txt", ios::out | ios::app);
//    if(!outFile){
//        //throw une exception
//        return -1;
//    }
//    double sec = (double)(clock() - tStart)/CLOCKS_PER_SEC;
//    outFile<<"Temps d'execution = "<<(int)(sec/60)<<":"<<(int)sec%60 + sec-(int)sec<<endl<<endl;
//    outFile.close();

    return 0;
}


