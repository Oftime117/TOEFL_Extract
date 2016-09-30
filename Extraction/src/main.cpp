#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

#include "essay.h"

using namespace std;

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

int main()
{
    ifstream file("data/train.txt", ios::in);

    if(!file)
    {
       cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    else
    {
        string line;
        //vector<Essay> essayList = vector<Essay>();
        unordered_map<string, int> dico;
        unordered_set<string> test;
        test.size();
        while (std::getline(file, line))
        {
            //getline(file, line);

            Essay e(line, dico);
            //essayList.push_back(e);
        }
        file.close();
        cout << dico.size() << endl;
//        for(auto &a: dico)
//        {
//            cout << a.first << " ";
//        }
    }

    return 0;
}


