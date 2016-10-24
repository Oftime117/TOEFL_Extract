#!/bin/bash
filename='trainwap_l'
fichierSortie=$1

#initialisation
line1=`cut -d$'\n' -f 1 $filename`
line2=`cut -d$'\n' -f 2 $filename`
line3=`cut -d$'\n' -f 3 $filename`
echo "$line1 $line2 $line3" > $fichierSortie

while read newline ;
do
	line1=$line2
	line2=$line3
	line3=$newline

	if [ -z "$newline" ]; then #on passe au texte suivant
		echo "">>$fichierSortie
		read line1 $filename
		read line2 $filename
		read line3 $filename
	fi

	echo "$line1 $line2 $line3" >> $fichierSortie
done < ${filename}
#./supprPonctu $fichierSortie "$fichierSortieNoPonct"
