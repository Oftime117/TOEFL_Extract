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
    chrono::time_point<chrono::system_clock> startTime, endTime;
    startTime = chrono::system_clock::now();

    //Entraînement en partant de rien

    Model modele("data/train.txt", "data/features.txt", "data/model.txt");

    endTime = chrono::system_clock::now();

    auto loadElapsedMilli = chrono::duration_cast<std::chrono::milliseconds>
                             (endTime-startTime).count();
    cout << "*** Temps de chargement des caracteristiques: " << loadElapsedMilli / 60000 << ":"
         << loadElapsedMilli/1000%60 << "." <<loadElapsedMilli % 1000 << " ***"<< endl << endl;
    cout <<"nb threads " << thread::hardware_concurrency() << endl;

    modele.trainByDiv3(10);

//    modele.resetConfusionMatrix();
//    modele.trainByDiv(10, 2);
//    modele.printConfusionMatrix("data/confusion.txt");

    startTime = chrono::system_clock::now();
    auto CVTrainElapsedMilli = chrono::duration_cast<std::chrono::milliseconds>
                             (startTime-endTime).count();
    cout << "*** Temps de l'entrainement: " << CVTrainElapsedMilli / 60000 << ":"
         << CVTrainElapsedMilli/1000%60 <<"."<< CVTrainElapsedMilli % 1000 << " ***"<< endl << endl;
/*
    //Estimer la qualité de l'entraînement
    Model modele2("data/train.txt", "data/features.txt", "data/model.txt");
    modele2.trainByDiv(10);
    modele2.save();
*/
/*
    //Test d'une solution en se servant d'un modèle entraîné
    Model m("data/features.txt", "data/model.txt");
    m.evaluer(unEssai, 0); //un essai non corrompu
    m.setOutFiles("data/features_2.txt", "data/model_2.txt");
    m.save();
*/
    endTime = chrono::system_clock::now();

    auto elapsedMilli = chrono::duration_cast<std::chrono::milliseconds>
                             (endTime-startTime).count();
    cout<<"Ellapsed time: "<<elapsedMilli/60000<<":"<<elapsedMilli/1000%60<<"."<<elapsedMilli%1000<<endl;


    ofstream outFile("data/history.txt", ios::out | ios::app);
    if(!outFile){
        //throw une exception
        return -1;
    }
    outFile<<"Temps d'execution = "<<elapsedMilli/60000<<":"<<elapsedMilli/1000%60<<"."<<elapsedMilli%1000<<endl<<endl;
    outFile.close();

    return 0;
}


