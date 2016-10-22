#include <string>
#include <sstream>

#include "essay.h"
#include "model.h"

using namespace std;





int main()
{
    //Entra�nement en partant de rien

    Model modele("data/train.txt", "data/features.txt", "data/model.txt");
    modele.trainByDiv(10);

/*
    //Estimer la qualit� de l'entra�nement
    Model modele2("data/train.txt", "data/features.txt", "data/model.txt");
    modele2.trainByDiv(10);
    modele2.save();
*/
/*
    //Test d'une solution en se servant d'un mod�le entra�n�
    Model m("data/features.txt", "data/model.txt");
    m.evaluer(m.getFirstEssayP()); //un essai non corrompu
    m.setOutFiles("data/features_2.txt", "data/model_2.txt");
    m.save();
*/
    return 0;
}


