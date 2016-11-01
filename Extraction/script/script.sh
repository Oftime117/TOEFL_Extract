#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
# le dossier wapiti principal est supposé dans le même répertoire que ce fichier
# le dossier data contenant train.txt aussi
# si l'opération a été interompu il faut supprimer les fichiers qui n'avaient pas finis d'être créés

# utilisation de wapiti
echo ">> Fabrication des labels"
if [ ! -f "trainTagList.txt" ] ; then
	./extract_l.sh ../data/train.txt trainTagList.txt
fi

echo ">> Transformation de la liste en ligne"
if [ ! -f "trainTagLine.txt" ] ; then
	./listToLine.sh trainTagList.txt trainTagLine.txt &
fi

# lancement des scripts comptant les occurences de types
echo ">> Lancement du calcul des occurences par texte pour un tag seul"
if [ ! -f "occurence1TagList.txt" ] ; then
	./groupe1.sh trainTagList.txt occurence1TagList.txt &
fi

echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
if [ ! -f "occurence2TagList.txt" ] ; then
	./groupe2.sh trainTagList.txt occurence2TagList.txt &
fi

echo ">> Lancement du calcul des occurences par texte pour trois labels consécutifs"
if [ ! -f "occurence3TagList.txt" ] ; then
	./groupe3.sh trainTagList.txt occurence3TagList.txt &
fi

echo ">> En attente de la fin des calculs"
wait # les calculs sont lancés en parallèle, on attend ensuite la fin de ceux-ci

# on les transforme en ligne pour les parser plus facilement ensuite
echo ">> Transformation de la liste des labels seuls en ligne"
if [ ! -f "occurence1TagLine.txt" ] ; then
	./occurenceToLine.sh occurence1TagList.txt occurence1TagLine.txt &
fi

echo ">> Transformation de la liste des labels par deux en ligne"
if [ ! -f "occurence2TagLine.txt" ] ; then
	./occurenceToLine.sh occurence2TagList.txt occurence2TagLine.txt &
fi

echo ">> Transformation de la liste des labels par trois en ligne"
if [ ! -f "occurence3TagLine.txt" ] ; then
	./occurenceToLine.sh occurence3TagList.txt occurence3TagLine.txt &
fi

echo ">> En attente de la fin des tranformations"
wait
echo ">> Toutes les opérations sont terminées"
