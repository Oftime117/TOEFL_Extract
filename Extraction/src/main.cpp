#include <string>
#include <sstream>

#include "essay.h"
#include "model.h"

using namespace std;





int main()
{
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
    m.evaluer(m.getFirstEssayP()); //un essai non corrompu
    m.setOutFiles("data/features_2.txt", "data/model_2.txt");
    m.save();
*/
    return 0;
}


