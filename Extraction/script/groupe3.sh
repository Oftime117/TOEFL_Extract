#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
datatrainLoc=../data/train.txt
filename=$1 # $1 représente un fichier traité par grep "(" $1 | cut -d ")" -f 2- | tr " " "\n"  
			# OU par ./extract_l.sh ../data/train.txt trainTagList.txt
outfile=$2 	# $2 est le nom du fichier de sortie

> $outfile # creation du fichier de sortie

let "numline = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

> temp3.txt
> list3.txt

line1=""
line2=""
line3=""

while read newline ;
do
	
	if [ -z "$newline" ]; then # on passe au texte suivant
		# traitement à la vollée
		sed -n "$numline p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		#./supprPonctu.sh temp3.txt temp3NoPonct.txt # on enlève la ponctuation
		sort temp3.txt | uniq -c | sort -bnr >> $outfile # calcul des occurences
		echo "" >> $outfile
		
		sed -n "$numline p" listLanguage >> list3.txt # ajout de la langue qui correspond au groupe
		cat temp3.txt >> list3.txt
		echo "" >> list3.txt
		
		> temp3.txt
		
		let "numline = numline + 1"
		line1=""
		line2=""
		line3=""
	else
		if [ "$line3" = "" ]; then # on commence un nouvel essai
			line3=$newline
		elif [ "$line2" = "" ]; then # deuxième mot
			line2=$line3
			line3=$newline
		else # fonctionnement normal à partir du deuxème mot
			line1=$line2
			line2=$line3
			line3=$newline
			echo "$line1 $line2 $line3" >> temp3.txt
		fi
	fi
	
done < ${filename}

rm temp3.txt
#rm temp3NoPonct.txt

echo ">> Fin du calcul des occurences par texte pour trois entités consécutives"
