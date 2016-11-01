#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
datatrainLoc=../data/train.txt
filename=$1 # $1 représente train.txt
outfile=$2 # $2 est le nom du fichier de sortie

> $outfile # creation du fichier de sortie

let "numline = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

# initialisation

> temp1.txt

while read newline ;
do

	if [ -z "$newline" ]; then # on passe au texte suivant
		# traitement à la vollée
		sed -n "$numline p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		let "numline = numline + 1"
		./supprPonctu.sh temp1.txt temp1NoPonct.txt
		sort temp1NoPonct.txt | uniq -c | sort -bnr >> $outfile
		echo "" >> $outfile
		
		> temp1.txt
		
		read newline < ${filename}
	fi

	echo "$newline" >> temp1.txt
done < ${filename}

rm temp1.txt
rm temp1NoPonct.txt

echo ">> Fin du calcul des occurences par texte pour un label seul"


