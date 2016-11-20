#include "tools.h"

using namespace std;

float Tools::floatArrayAVG(const float tab[], const size_t& taille)
{
    if(taille == 0 ) return -1;
    float somme = 0.f;
    for(size_t i=0; i<taille; i++)
    {
        somme += tab[i];
    }
    return somme/static_cast<float>(taille);
}

size_t Tools::getMaxIndex(const int tab[], const size_t& taille)
{
    size_t maxInd = 0;
    for(size_t i=1; i<taille; i++){
        if(tab[i]>tab[maxInd]) maxInd = i;
    }
    return maxInd;
}

int Tools::getVectorIndex(const vector<string>& vec, const string& value)
{
    for(size_t i=0; i<vec.size(); i++){
        if(vec[i] == value) return i;
    }
    return -1;
}

bool Tools::addIfAbsent(std::map<std::string, int> &myMap, const string &val){
    try{
        myMap.at(val);
    }
    catch(out_of_range &oor){
        myMap.emplace(val, myMap.size());
        return true;
    }
    return false;
}

void Tools::sumMatrix(std::vector<std::vector<int>> &mat, const std::vector<std::vector<int>> &matConst)
{
    if(mat.size() != matConst.size()) return;
    for(size_t i=0; i<mat.size(); i++)
    {
        if(mat[i].size() != matConst[i].size()) return;
        for(size_t j=0; j<mat[i].size(); j++)
        {
            mat[i][j] += matConst[i][j];
        }
    }
}
