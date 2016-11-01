#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
datatrainLoc=../data/train.txt
filename=$1 # $1 représente train.txt
outfile=$2 # $2 est le nom du fichier de sortie

> $outfile # creation du fichier de sortie

let "numline = 1"
grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

# initialisation
line1=`cut -d$'\n' -f 1 $filename`
line2=`cut -d$'\n' -f 2 $filename`
line3=`cut -d$'\n' -f 3 $filename`
echo "$line1 $line2 $line3" > temp3.txt
> list3.txt

i=1
while read newline ;
do
	test $i -eq 1 && let "i = i + 1" && continue
	test $i -eq 2 && let "i = i + 1" && continue
	test $i -eq 3 && let "i = i + 1" && continue
	line1=$line2
	line2=$line3
	line3=$newline

	if [ -z "$newline" ]; then # on passe au texte suivant
		# traitement à la vollée
		sed -n "$numline p" listLanguage >> $outfile # ajout de la langue qui correspond au groupe
		./supprPonctu.sh temp3.txt temp3NoPonct.txt # on enlève la ponctuation
		sort temp3NoPonct.txt | uniq -c | sort -bnr >> $outfile # calcul des occurences
		echo "" >> $outfile
		
		sed -n "$numline p" listLanguage >> list3.txt # ajout de la langue qui correspond au groupe
		cat temp3.txt >> list3.txt
		echo "" >> list3.txt
		
		> temp3.txt
		
		let "numline = numline + 1"
		read line1 < ${filename}
		read line2 < ${filename}
		read line3 < ${filename}
	fi

	echo "$line1 $line2 $line3" >> temp3.txt
done < ${filename}

rm temp3.txt
rm temp3NoPonct.txt

echo ">> Fin du calcul des occurences par texte pour trois labels consécutifs"
