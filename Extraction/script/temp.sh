#!/bin/bash

while read newline ;
do
	valeur=`echo "${newline}" | cut -d" " -f 1`
	type=`echo "${newline}" | cut -d" " -f 2`
	if [ ! "$type" ] ; then
		echo "pas de type pour la valeur: $valeur"
		continue
	fi
	valeurs["$type"]=$valeur
done < valeursGroupe3

echo "la moitié"

cat types-sum2-valeursGroupe3.txt | tr "$" "~" > types-sum2-valeursGroupe3_temp.txt

while read newline ;
do
	type=`echo "${newline}" | cut -d" " -f 1`
	sum2=`echo "${newline}" | cut -d" " -f 2`
	echo $type $sum2 ${valeurs["$type"]} >> types-sum2-valeursGroupe3_temp.txt
done < types-sum2-valeursGroupe3.txt

echo "fini"
