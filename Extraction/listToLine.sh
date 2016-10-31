#!/bin/bash
#Florian TALOUR & Tristan LE NAIR

datatrainLoc=data/train.txt
filename=$1 # $1 est la sortie de extract_l.sh
outfile=$2 # $2 est le nom de la sortie pour la liste des tags dans le même format que train.txt

#Partie 1: ajoute la language qui correspond à l'essay
let "numline = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

#initialisation
newline=`cut -d$'\n' -f 1 $filename`
sed -n "$numline p" listLanguage > $outfile # ajout de la langue qui correspond au groupe
let "numline = numline + 1"
echo "$newline" >> $outfile

while read newline ;
do

	if [ -z "$newline" ]; then #on passe au texte suivant
		#traitement à la vollée
		echo "" >> $outfile
		
		sed -n "$numline p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		let "numline = numline + 1"
		
		read newline $filename
	fi
	
	echo "$newline" >> $outfile

done < ${filename}

#Partie 2: Transforme la liste en formatant comme train.txt
cat $outfile | tr "\n" " " | tr "(" "\n" > $outfile
sed -i '1d' $outfile
sed -i 's/^/(/' $outfile

