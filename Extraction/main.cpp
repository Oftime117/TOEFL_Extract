#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream file("/home/Documents/data/train.txt", ios::in);

    if(file)
    {
        string line;
        vector<string> listOfWords;

        while (std::getline(file, line)) // one line -> when '\n' is found
        {
            std::size_t found = line.find_first_of(' ', 10); // (xxx,low)
            string line_cut = line.substr(found+1); // line cut do not contain the first word
            split(line_cut, ' ', listOfWords);
        }

        fichier.close();

        std::cout << listOfWords << endl;

    }
    else
    {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }

    return 0;
}

void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
