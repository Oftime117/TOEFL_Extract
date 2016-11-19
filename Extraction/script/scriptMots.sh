#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
# le dossier wapiti principal est supposé dans le même répertoire que ce fichier
# le dossier data contenant train.txt aussi
# si l'opération a été interompu il faut supprimer les fichiers qui n'avaient pas finis d'être créés
train=../data/train.txt

if [ "$1" -eq "1" ]; then # supprimer tout
	rm -f trainMotList.txt trainMotLine.txt occurence1MotList.txt occurence2MotList.txt occurence3MotList.txt occurence1MotLine.txt occurence2MotLine.txt occurence3MotLine.txt occurence1MotListLower.txt occurence2MotListLower.txt occurence3MotListLower.txt occurence1MotLineLower.txt occurence2MotLineLower.txt occurence3MotLineLower.txt
elif [ "$1" -eq "2" ]; then # supprimer tout sauf trainMotList.txt
	rm -f trainMotLine.txt occurence1MotList.txt occurence2MotList.txt occurence3MotList.txt occurence1MotLine.txt occurence2MotLine.txt occurence3MotLine.txt occurence1MotListLower.txt occurence2MotListLower.txt occurence3MotListLower.txt occurence1MotLineLower.txt occurence2MotLineLower.txt occurence3MotLineLower.txt
elif [ "$1" -eq "3" ]; then # supprimer tout ce qui se rapporte aux majuscules
	rm -f trainMotLine.txt occurence1MotList.txt occurence2MotList.txt occurence3MotList.txt occurence1MotLine.txt occurence2MotLine.txt occurence3MotLine.txt
elif [ "$1" -eq "4" ]; then # supprimer tout ce qui se rapporte aux minuscules
	rm -f trainMotLine.txt occurence1MotListLower.txt occurence2MotListLower.txt occurence3MotListLower.txt occurence1MotLineLower.txt occurence2MotLineLower.txt occurence3MotLineLower.txt
elif [ "$1" -eq "5" ]; then # supprimer tout ce qui se rapporte aux minuscules
	rm -f trainMotLine.txt occurence1MotLine.txt occurence2MotLine.txt occurence3MotLine.txt occurence1MotLineLower.txt occurence2MotLineLower.txt occurence3MotLineLower.txt
fi

grep "(" $train | cut -d ")" -f 2- | tr " " "\n" > trainMotList.txt
sed -i '1d' trainMotList.txt # supprime la ligne vide créée
printf "%b\n" >> trainMotList.txt

cat trainMotList.txt | tr '[:upper:]' '[:lower:]' > trainMotListLower.txt

# lancement des scripts comptant les occurences d'entités avec majuscule
if [ ! -f "occurence1MotList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour un Mot seul"
	./groupe1.sh trainMotList.txt occurence1MotList.txt &
fi

if [ ! -f "occurence2MotList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
	./groupe2.sh trainMotList.txt occurence2MotList.txt &
fi

if [ ! -f "occurence3MotList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour trois labels consécutifs"
	./groupe3.sh trainMotList.txt occurence3MotList.txt &
fi

echo ">> En attente de la fin des calculs pour les mots avec majuscule"
wait

# lancement des scripts comptant les occurences d'entités sans majuscule
if [ ! -f "occurence1MotListLower.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour un Mot seul"
	./groupe1.sh trainMotListLower.txt occurence1MotListLower.txt &
fi

if [ ! -f "occurence2MotListLower.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
	./groupe2.sh trainMotListLower.txt occurence2MotListLower.txt &
fi

if [ ! -f "occurence3MotListLower.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour trois labels consécutifs"
	./groupe3.sh trainMotListLower.txt occurence3MotListLower.txt &
fi

echo ">> En attente de la fin des calculs pour les mots sans majuscules"
wait # les calculs sont lancés en parallèle, on attend ensuite la fin de ceux-ci

# on les transforme en ligne pour les parser plus facilement ensuite, avec majuscule
if [ ! -f "occurence1MotLine.txt" ] ; then
	echo ">> Transformation de la liste des labels seuls en ligne"
	./occurenceToLine.sh occurence1MotList.txt occurence1MotLine.txt &
fi

if [ ! -f "occurence2MotLine.txt" ] ; then
	echo ">> Transformation de la liste des labels par deux en ligne"
	./occurenceToLine.sh occurence2MotList.txt occurence2MotLine.txt &
fi

if [ ! -f "occurence3MotLine.txt" ] ; then
	echo ">> Transformation de la liste des labels par trois en ligne"
	./occurenceToLine.sh occurence3MotList.txt occurence3MotLine.txt &
fi

# on les transforme en ligne pour les parser plus facilement ensuite, sans majuscule
if [ ! -f "occurence1MotLineLower.txt" ] ; then
	echo ">> Transformation de la liste des labels seuls en ligne"
	./occurenceToLine.sh occurence1MotListLower.txt occurence1MotLineLower.txt &
fi

if [ ! -f "occurence2MotLineLower.txt" ] ; then
	echo ">> Transformation de la liste des labels par deux en ligne"
	./occurenceToLine.sh occurence2MotListLower.txt occurence2MotLineLower.txt &
fi

if [ ! -f "occurence3MotLineLower.txt" ] ; then
	echo ">> Transformation de la liste des labels par trois en ligne"
	./occurenceToLine.sh occurence3MotListLower.txt occurence3MotLineLower.txt &
fi

echo ">> En attente de la fin des tranformations"
wait
echo ">> Toutes les opérations sont terminées"
