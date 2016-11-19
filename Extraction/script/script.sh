#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
# le dossier wapiti principal est supposé dans le même répertoire que ce fichier
# le dossier data contenant train.txt aussi
# si l'opération a été interompu il faut supprimer les fichiers qui n'avaient pas finis d'être créés

if [ "$1" -eq "1" ]; then # supprime tout
	rm -f trainTagList.txt trainTagLine.txt occurence1TagList.txt occurence2TagList.txt occurence3TagList.txt occurence1TagLine.txt occurence2TagLine.txt occurence3TagLine.txt
elif [ "$1" -eq "2" ]; then # supprime tout sauf trainTagList.txt
	rm -f trainTagLine.txt occurence1TagList.txt occurence2TagList.txt occurence3TagList.txt occurence1TagLine.txt occurence2TagLine.txt occurence3TagLine.txt
fi

# utilisation de wapiti
if [ ! -f "trainTagList.txt" ] ; then
	echo ">> Fabrication des labels"
	./extract_l.sh ../data/train.txt trainTagList.txt
fi

if [ ! -f "trainTagLine.txt" ] ; then
	echo ">> Transformation de la liste en ligne"
	./listToLine.sh trainTagList.txt trainTagLine.txt &
fi

# lancement des scripts comptant les occurences de types
if [ ! -f "occurence1TagList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour un tag seul"
	./groupe1.sh trainTagList.txt occurence1TagList.txt &
fi

if [ ! -f "occurence2TagList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
	./groupe2.sh trainTagList.txt occurence2TagList.txt &
fi

if [ ! -f "occurence3TagList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour trois labels consécutifs"
	./groupe3.sh trainTagList.txt occurence3TagList.txt &
fi

echo ">> En attente de la fin des calculs"
wait # les calculs sont lancés en parallèle, on attend ensuite la fin de ceux-ci

# on les transforme en ligne pour les parser plus facilement ensuite
if [ ! -f "occurence1TagLine.txt" ] ; then
	echo ">> Transformation de la liste des labels seuls en ligne"
	./occurenceToLine.sh occurence1TagList.txt occurence1TagLine.txt &
fi

if [ ! -f "occurence2TagLine.txt" ] ; then
	echo ">> Transformation de la liste des labels par deux en ligne"
	./occurenceToLine.sh occurence2TagList.txt occurence2TagLine.txt &
fi

if [ ! -f "occurence3TagLine.txt" ] ; then
	echo ">> Transformation de la liste des labels par trois en ligne"
	./occurenceToLine.sh occurence3TagList.txt occurence3TagLine.txt &
fi

echo ">> En attente de la fin des tranformations"
wait
if [ -f listLanguage ]; then
	rm listLanguage
fi
echo ">> Toutes les opérations sont terminées"
