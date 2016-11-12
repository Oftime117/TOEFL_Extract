#include <string>
#include <sstream>
#include <iostream>
#include <iostream>
#include <fstream>
#include <chrono>

#include "essay.h"
#include "model.h"

using namespace std;


int main(int argc, char** argv)
{
    chrono::time_point<chrono::system_clock> startTime, endTime;
    startTime = chrono::system_clock::now();

    //Entraînement en partant de rien

    Model modele("data/train.txt", "data/features.txt", "data/model.txt");
    modele.resetConfusionMatrix();
    modele.trainByDiv(10, 10);
    modele.printConfusionMatrix("data/confusion.txt");



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


