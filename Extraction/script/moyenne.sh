#!/bin/bash
#Florian TALOUR & Tristan LE NAIR
declare -A moyenne
#nombre total d'éléments par type (calcul de la moyenne)
#grep "(" data/train.txt | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > temp23.txt
#sort script/sortie.txt | uniq -c | sort -bnr > moy

sed -i 's/^\ \ *//' moy.txt
grep . moy.txt | cut -d" " -f 1 > valeurs
echo ${valeurs}
grep . moy.txt | cut -d" " -f 2 > types
echo ${types}

i=1
while read newline ;
do
	valeur=`sed -n "$i p" valeurs`
	type=`sed -n "$i p" types`
	#let "i = i + 1"
	`let "moyenne["$type"] = valeur / 9900"`
done < moy

echo ${moyenne[*]}
#calcul de l'écart type
datatrainLoc=data/train.txt

#> $outfile # creation du fichier de sortie

let "numline = 1"
SC=9
#grep "(" $datatrainLoc | cut -d " " -f -1 > listLanguage

#initialisation
#newline=`cut -d$'\n' -f 1 temp23.txt`
n=9900

grep . $1 | cut -d" " -f 1 > valeurs2
grep . $1 | cut -d" " -f 2 > types2

declare -A dif1
declare -A dif2
declare -A sum2 #calcul de la variance
declare -A avg2 #moyenne
declare -A sdev #ecart type
declare -A tab


sed '/^(/d' $1 > sanslan

j=1
while read nline ;
do

	if [ -n "$nline" ] ; then 
		valeur=`sed -n "$j p" valeurs2`
		type=`sed -n "$j p" types2`
		let "j = i +1"
		dif1["$type"]=$(echo "scale=$SC; ${moyenne["$type"]} - $valeur" | bc) # Différence avec la moyenne
		dif2["$type"]=$(echo "scale=$SC; ${dif1["$type"]} * ${dif1["$type"]}" | bc) # au carré
		sum2["$type"]=$(echo "scale=$SC; ${sum2["$type"]} + ${dif2["$type"]}" | bc) # somme
	fi

done < sanslan

${sum2[*]} | tr " " "\n" > var
for ty in var 
do
	avg2["$ty"]=$(echo "scale=$SC; ${sum2["$ty"]} / $n" | bc)  # moyenne des écarts
	sdev["$ty"]=$(echo "scale=$SC; sqrt(${avg2["$ty"]})" | bc) # racine carré
done

echo "$sdev[*]"

