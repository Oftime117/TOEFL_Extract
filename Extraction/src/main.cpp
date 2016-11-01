#include <string>
#include <sstream>
#include <iostream>
//#include <time.h> //calculer le temps d'execution
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

#include "essay.h"
#include "model.h"

using namespace std;


int main(int argc, char** argv)
{
    //clock_t tStart = clock();

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    //Entraînement en partant de rien

    Model modele("data/train.txt", "data/features.txt", "data/model.txt");
    //modele.trainByDiv(10);

    end = chrono::system_clock::now();

    auto elapsedMilli = chrono::duration_cast<std::chrono::milliseconds>
                             (end-start).count();
    cout << "Ellapsed time: " << elapsedMilli;

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


