#!/bin/bash
#Florian TALOUR & Tristan LE NAIR
datatrainLoc=data/train.txt
filename=$1
outfile=$2

> $outfile # creation du fichier de sortie

let "numline = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

#initialisation
newline=`cut -d$'\n' -f 1 $filename`
echo "$newline" > temp.txt

while read newline ;
do

	if [ -z "$newline" ]; then #on passe au texte suivant
		#traitement à la vollée
		sed -n "$numline p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		let "numline = numline + 1"
		./supprPonctu.sh temp.txt tempNoPonct.txt
		sort tempNoPonct.txt | uniq -c | sort -bnr >> $outfile
		echo "" >> $outfile
		
		> temp.txt
		
		read line $filename
	fi

	echo "$newline" >> temp.txt
done < ${filename}

