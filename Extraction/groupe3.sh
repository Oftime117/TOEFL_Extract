#!/bin/bash
datatrainLoc=data/train.txt
filename=$1
outfile=$2

> $outfile # creation du fichier de sortie

let "numline = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

#initialisation
line1=`cut -d$'\n' -f 1 $filename`
line2=`cut -d$'\n' -f 2 $filename`
line3=`cut -d$'\n' -f 3 $filename`
echo "$line1 $line2 $line3" > temp.txt
sed -n "$numline p" listLanguage > list3.txt # ajout de la langue qui correspond au groupe
let "numline = numline + 1"
echo "$line1 $line2 $line3" > list3.txt

while read newline ;
do
	line1=$line2
	line2=$line3
	line3=$newline

	if [ -z "$newline" ]; then #on passe au texte suivant
		#traitement à la vollée
		sed -n "$numline p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		let "numline = numline + 1"
		./supprPonctu.sh temp.txt tempNoPonct.txt
		sort tempNoPonct.txt | uniq -c | sort -bnr >> $outfile
		echo "" >> $outfile
		
		> temp.txt
		echo "" >> list3.txt
		
		sed -n "$numline p" listLanguage >> list3.txt # ajout de la langue qui correspond au groupe
		read line1 $filename
		read line2 $filename
		read line3 $filename
	fi

	echo "$line1 $line2 $line3" >> temp.txt
	echo "$line1 $line2 $line3" >> list3.txt
done < ${filename}
