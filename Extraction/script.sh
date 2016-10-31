#!/bin/bash
#Florian TALOUR & Tristan LE NAIR

#utilisation de wapiti
echo ">> Fabrication des tags"
./extract_l.sh data/train.txt trainTagList.txt

echo ">> Transformation de la liste en ligne"
./listToLine.sh trainTagList.txt trainTagLine.txt &

#lancement des scripts comptant les occurences de types
echo ">> Lancement du calcul des occurences par texte pour un tag seul"
./groupe1.sh trainTagList.txt occurence1TagList.txt &
echo ">> Lancement du calcul des occurences par texte pour deux tags consécutifs"
./groupe2.sh trainTagList.txt occurence2TagList.txt &
echo ">> Lancement du calcul des occurences par texte pour trois tags consécutifs"
./groupe3.sh trainTagList.txt occurence3TagList.txt &

echo ">> En attente de la fin des calculs"
wait # les calculs sont lancés en parallèle, on attend ensuite la fin de ceux-ci

#on les transforme en ligne pour les parser plus facilement ensuite
echo ">> Transformation de la liste des tags seuls en ligne"
./occurenceToLine.sh occurence1TagList.txt occurence1TagLine.txt
echo ">> Transformation de la liste des tags par deux en ligne"
./occurenceToLine.sh occurence2TagList.txt occurence2TagLine.txt
echo ">> Transformation de la liste des tags par trois en ligne"
./occurenceToLine.sh occurence3TagList.txt occurence3TagLine.txt
