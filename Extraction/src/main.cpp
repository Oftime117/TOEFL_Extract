#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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

    if(file)
    {
        string line;
        vector<Essay> essayList = vector<Essay>();

        /*while (std::getline(file, line))*/ // one line -> when '\n' is found
        {
            getLine(file, line);
            Essay e(line);
            essayList.push_back(e);
        }
        file.close();

        cout << "list: " << essayList.size() << endl;
        for(Essay &e : essayList)
        {
            cout << "Essay size: " << e.getText().size() << endl;
            cout << e << endl;

            unordered_map<string, int> dictionary;
            int i = 0;

            for(string word : e.getWordsList())
            {
                dictionary.insert(pair(word, i));
                i++;
            }
        }
    }
    else
    {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }


    return 0;
}


