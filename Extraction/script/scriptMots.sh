#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
# le dossier wapiti principal est supposé dans le même répertoire que ce fichier
# le dossier data contenant train.txt aussi
# si l'opération a été interompu il faut supprimer les fichiers qui n'avaient pas finis d'être créés
train=../data/train.txt

grep "(" $train | cut -d ")" -f 2- | tr " " "\n" > trainMotList.txt
sed -i '0,/^$/d' trainMotList.txt
cat trainMotList.txt | tr '[:upper:]' '[:lower:]' > trainMotListLower.txt


# lancement des scripts comptant les occurences d'entités avec majuscule
echo ">> Lancement du calcul des occurences par texte pour un Mot seul"
if [ ! -f "occurence1MotList.txt" ] ; then
	./groupe1.sh trainMotList.txt occurence1MotList.txt &
fi

echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
if [ ! -f "occurence2MotList.txt" ] ; then
	./groupe2.sh trainMotList.txt occurence2MotList.txt &
fi

echo ">> Lancement du calcul des occurences par texte pour trois labels consécutifs"
if [ ! -f "occurence3MotList.txt" ] ; then
	./groupe3.sh trainMotList.txt occurence3MotList.txt &
fi

# lancement des scripts comptant les occurences d'entités sans majuscule
echo ">> Lancement du calcul des occurences par texte pour un Mot seul"
if [ ! -f "occurence1MotListLower.txt" ] ; then
	./groupe1.sh trainMotListLower.txt occurence1MotListLower.txt &
fi

echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
if [ ! -f "occurence2MotListLower.txt" ] ; then
	./groupe2.sh trainMotListLower.txt occurence2MotListLower.txt &
fi

echo ">> Lancement du calcul des occurences par texte pour trois labels consécutifs"
if [ ! -f "occurence3MotListLower.txt" ] ; then
	./groupe3.sh trainMotListLower.txt occurence3MotListLower.txt &
fi

echo ">> En attente de la fin des calculs"
wait # les calculs sont lancés en parallèle, on attend ensuite la fin de ceux-ci

# on les transforme en ligne pour les parser plus facilement ensuite, avec majuscule
echo ">> Transformation de la liste des labels seuls en ligne"
if [ ! -f "occurence1MotLine.txt" ] ; then
	./occurenceToLine.sh occurence1MotList.txt occurence1MotLine.txt &
fi

echo ">> Transformation de la liste des labels par deux en ligne"
if [ ! -f "occurence2MotLine.txt" ] ; then
	./occurenceToLine.sh occurence2MotList.txt occurence2MotLine.txt &
fi

echo ">> Transformation de la liste des labels par trois en ligne"
if [ ! -f "occurence3MotLine.txt" ] ; then
	./occurenceToLine.sh occurence3MotList.txt occurence3MotLine.txt &
fi

# on les transforme en ligne pour les parser plus facilement ensuite, sans majuscule
echo ">> Transformation de la liste des labels seuls en ligne"
if [ ! -f "occurence1MotLine.txt" ] ; then
	./occurenceToLine.sh occurence1MotListLower.txt occurence1MotLineLower.txt &
fi

echo ">> Transformation de la liste des labels par deux en ligne"
if [ ! -f "occurence2MotLine.txt" ] ; then
	./occurenceToLine.sh occurence2MotListLower.txt occurence2MotLineLower.txt &
fi

echo ">> Transformation de la liste des labels par trois en ligne"
if [ ! -f "occurence3MotLine.txt" ] ; then
	./occurenceToLine.sh occurence3MotListLower.txt occurence3MotLineLower.txt &
fi

echo ">> En attente de la fin des tranformations"
wait
echo ">> Toutes les opérations sont terminées"
