#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
datatrainLoc=../data/train.txt
filename=$1 # $1 représente train.txt
outfile=$2 # $2 est le nom du fichier de sortie

> $outfile # creation du fichier de sortie

let "numline2 = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

# initialisation
line1=`cut -d$'\n' -f 1 $filename`
line2=`cut -d$'\n' -f 2 $filename`
echo "$line1 $line2" > temp2.txt

sed -n "$numline2 p" listLanguage > list2.txt # ajout de la langue qui correspond au groupe
let "numline2 = numline2 + 1"
echo "$line1 $line2" >> list2.txt

sed 2d $filename | while read newline ;
do
	line1=$line2
	line2=$newline

	if [ -z "$newline" ]; then # on passe au texte suivant
		# traitement à la vollée
		sed -n "$numline2 p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		let "numline2 = numline2 + 1"
		./supprPonctu.sh temp2.txt temp2NoPonct.txt
		sort temp2NoPonct.txt | uniq -c | sort -bnr >> $outfile
		echo "" >> $outfile
		
		> temp2.txt
		echo "" >> list2.txt
		
		sed -n "$numline2 p" listLanguage >> list2.txt # ajout de la langue qui correspond au groupe
		read line1 < ${filename}
		read line2 < ${filename}
	fi

	echo "$line1 $line2" >> temp2.txt
	echo "$line1 $line2" >> list2.txt
done

rm temp2.txt
rm temp2NoPonct.txt

echo ">> Fin du calcul des occurences par texte pour deux labels consécutifs"