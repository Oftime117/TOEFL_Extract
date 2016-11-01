#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
datatrainLoc=../data/train.txt
filename=$1 # $1 représente train.txt
outfile=$2 # $2 est le nom du fichier de sortie

> $outfile # creation du fichier de sortie

let "numline3 = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

# initialisation
line1=`cut -d$'\n' -f 1 $filename`
line2=`cut -d$'\n' -f 2 $filename`
line3=`cut -d$'\n' -f 3 $filename`
echo "$line1 $line2 $line3" > temp3.txt

sed -n "$numline3 p" listLanguage > list3.txt # ajout de la langue qui correspond au groupe
let "numline3 = numline3 + 1"
echo "$line1 $line2 $line3" >> list3.txt

sed 3d $filename | while read newline ;
do
	line1=$line2
	line2=$line3
	line3=$newline

	if [ -z "$newline" ]; then # on passe au texte suivant
		# traitement à la vollée
		sed -n "$numline3 p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		let "numline3 = numline3 + 1"
		./supprPonctu.sh temp3.txt temp3NoPonct.txt
		sort temp3NoPonct.txt | uniq -c | sort -bnr >> $outfile
		echo "" >> $outfile
		
		> temp3.txt
		echo "" >> list3.txt
		
		sed -n "$numline3 p" listLanguage >> list3.txt # ajout de la langue qui correspond au groupe
		read line1 < ${filename}
		read line2 < ${filename}
		read line3 < ${filename}
	fi

	echo "$line1 $line2 $line3" >> temp3.txt
	echo "$line1 $line2 $line3" >> list3.txt
done

rm temp3.txt
rm temp3NoPonct.txt

echo ">> Fin du calcul des occurences par texte pour trois labels consécutifs"
