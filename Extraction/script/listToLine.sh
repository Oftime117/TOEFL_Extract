#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
datatrainLoc=../data/train.txt
filename=$1 #  $1 est la sortie de extract_l.sh
outfile=$2 #  $2 est le nom de la sortie pour la liste des tags dans le même format que train.txt

# Partie 1: ajoute la language qui correspond à l'essay
let "numline0 = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

# initialisation
sed -n "$numline0 p" listLanguage > temp0.txt # ajout de la langue qui correspond au groupe
let "numline0 = numline0 + 1"

while read newline ;
do

	if [ -z "$newline" ]; then # on passe au texte suivant
		# traitement à la vollée
		echo "" >> temp0.txt
		sed -n "$numline0 p" listLanguage >> temp0.txt # ajout de la langue qui correspond au groupe
		let "numline0 = numline0 + 1"
		
		read newline < ${filename}
	fi
	
	echo "$newline" >> temp0.txt

done < ${filename}

# Partie 2: Transforme la liste en formatant comme train.txt
./occurenceToLine.sh temp0.txt $outfile

rm temp0.txt

echo ">> Fin de la transformation de la liste en ligne"
