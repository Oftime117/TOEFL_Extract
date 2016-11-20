#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>


#include "essay.h"
#include "model.h"

using namespace std;

string printTempsEcoulee(chrono::time_point<chrono::system_clock> startTime,
                       chrono::time_point<chrono::system_clock> endTime)
{
    auto elapsedMilli = chrono::duration_cast<std::chrono::milliseconds>
                             (endTime-startTime).count();
    stringstream result;
    result << elapsedMilli/60000 << ":" << elapsedMilli/1000%60 << "." << elapsedMilli%1000;
    return result.str();

}

int main(/*int argc, char** argv*/)
{
    chrono::time_point<chrono::system_clock> t1, t2, t3;
    t1 = chrono::system_clock::now();

    //Entraînement en partant de rien

    Model modele("data/train.txt", "data/features.txt", "data/model.txt");

    t2 = chrono::system_clock::now();

    cout << "*** Temps de chargement des caracteristiques: " << printTempsEcoulee(t1, t2) << " ***\n\n";
    cout <<"nb threads " << thread::hardware_concurrency() << endl;

    modele.resetConfusionMatrix();
    modele.trainByDiv2(10);
    modele.printConfusionMatrix("data/confusion.txt");

    t3 = chrono::system_clock::now();
    cout << "*** Temps de l'entrainement: " << printTempsEcoulee(t2, t3) << " ***\n\n";
/*
    //Estimer la qualité de l'entraînement
    Model modele2("data/train.txt", "data/features.txt", "data/model.txt");
    modele2.trainByDiv(10, 1);
    modele2.ssave();
*/
/*
    //Test d'une solution en se servant d'un modèle entraîné
    Model m("data/features.txt", "data/model.txt");
    m.evaluer(unEssai, 0); //un essai non corrompu
    m.setOutFiles("data/features_2.txt", "data/model_2.txt");
    m.save();
*/
    cout<<"*** Temps total: " << printTempsEcoulee(t1, t3) << " ***\n\n";


    ofstream outFile("data/history.txt", ios::out | ios::app);
    if(!outFile){
        //throw une exception
        return -1;
    }
    outFile<<"Temps d'execution = "<<printTempsEcoulee(t1, t3)<<endl<<endl;
    outFile.close();

    return 0;
}


