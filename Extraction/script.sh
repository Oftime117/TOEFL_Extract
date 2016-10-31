#!/bin/bash
#Florian TALOUR & Tristan LE NAIR


#utilisation de wapiti
grep "(" data/train.txt | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > tempo.txt

#lancement des scripts comptant les occurences de types
./groupe1.sh tempo.txt sortie1List.txt
./occurenceToLine.sh sortie1List.txt sortie1Line.txt

./groupe2.sh tempo.txt sortie2List.txt
./occurenceToLine.sh sortie2List.txt sortie2Line.txt

./groupe3.sh tempo.txt sortie3List.txt
./occurenceToLine.sh sortie3List.txt sortie3Line.txt
