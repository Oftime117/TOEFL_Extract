#include "tools.h"

using namespace std;

float Tools::floatArrayAVG(const float tab[], const size_t taille)
{
    if(taille == 0 ) return -1;
    float somme = 0.f;
    for(size_t i=0; i<taille; i++)
    {
        somme += tab[i];
    }
    return somme/(float)taille;
}

int Tools::getMaxIndex(const int tab[], const size_t taille)
{
    int max = 0;
    for(size_t i=1; i<taille; i++){
        if(tab[i]>tab[max]) max = i;
    }
    return max;
}


int Tools::getVectorIndex(const vector<string> vec, const string& value)
{
    for(size_t i=0; i<vec.size(); i++){
        if(vec[i] == value) return i;
    }
    return -1;
}

