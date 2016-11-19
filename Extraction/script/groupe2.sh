#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
datatrainLoc=../data/train.txt
filename=$1 # $1 représente un fichier traité par grep "(" $1 | cut -d ")" -f 2- | tr " " "\n"  
			# OU par ./extract_l.sh ../data/train.txt trainTagList.txt
outfile=$2 	# $2 est le nom du fichier de sortie

> $outfile # creation du fichier de sortie

let "numline = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

> temp2.txt
> list2.txt

line1=""
line2=""

while read newline ;
do
	
	if [ -z "$newline" ]; then # on passe au texte suivant
		sed -n "$numline p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		#./supprPonctu.sh temp2.txt temp2NoPonct.txt # on enlève la ponctuation
		sort temp2.txt | uniq -c | sort -bnr >> $outfile # calcul des occurences
		echo "" >> $outfile
		
		sed -n "$numline p" listLanguage >> list2.txt # ajout de la langue qui correspond au groupe
		cat temp2.txt >> list2.txt
		echo "" >> list2.txt
		
		> temp2.txt
		
		let "numline = numline + 1"
		line1=""
		line2=""
	else
		if [ "$line2" = "" ]; then # on commence un nouvel essai
			line2=$newline
		else # fonctionnement normal à partir du deuxième mot
			line1=$line2
			line2=$newline
			echo "$line1 $line2" >> temp2.txt
		fi
	fi
	
done < ${filename}

rm temp2.txt
#rm temp2NoPonct.txt

echo ">> Fin du calcul des occurences par texte pour deux entités consécutives"
