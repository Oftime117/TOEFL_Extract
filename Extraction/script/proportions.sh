#!/bin/bash
# Florian TALOUR & Tristan LE NAIR

declare -A proportion

> proportion1MotListLower.txt

SC=9
numtext=1
nbMotsTexte=`sed -n "1 p" nbMotTexte.txt`

while read newline ;
do
		
	if [ -z "$newline" ]; then # texte suivant
		let "numtext = numtext + 1"
		echo "" >> proportion1MotListLower.txt
		nbMotsTexte=`sed -n "$numtext p" nbMotTexte.txt`
	else
		valeur=`echo "${newline}" | cut -d" " -f 1`
		type=`echo "${newline}" | cut -d" " -f 2`
		if [ "$type" != "$valeur" ]; then
			echo "`echo "scale=$SC; $valeur/$nbMotsTexte" | bc` $type" >> proportion1MotListLower.txt
		else
			echo $newline >> proportion1MotListLower.txt
		fi
	fi
		
done < occurence1MotListLower.txt