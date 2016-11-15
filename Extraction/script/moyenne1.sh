#!/bin/bash
# Florian TALOUR & Tristan LE NAIR

########################
# calcul de la moyenne #
########################

if [ ! -f "trainTagList.txt" ] ; then
	./extract_l.sh ../data/train.txt trainTagList.txt
fi

# créaction des fichiers des moyennes
#if [ ! -f "listeMoyenne" ] ; then
	sort trainTagList.txt | uniq -c | sort -bnr > listeMoyenne
	sed -i 's/^\ \ *//' listeMoyenne
	sed -i '0,/^\ /s///' listeMoyenne
	sed -i '/^9900/d' listeMoyenne
#fi

# extraction pour contruire un tableau
grep . listeMoyenne | cut -d" " -f 1 > valeurs
grep . listeMoyenne | cut -d" " -f 2 > types

# creation tableau moyenne | valeur
declare -A moyenne

i=1
SC=9 # on prend 9 chiffres après la virgule
n=9900 # nombre de ligne dans ..data/train/train.txt
valeurMoyenne=0
while read newline ;
do
	valeur=`sed -n "$i p" valeurs`
	type=`sed -n "$i p" types`
	moyenne[$type]=$(echo "scale=$SC; $valeur / $n" | bc)
	echo $valeur $type ${moyenne[$type]}
	let "i = i + 1"
done < listeMoyenne

#echo ${moyenne[*]}

#############################
# calcul de l'écart type	#
#############################

# initialisation

# decalrion des tableaux associatifs
declare -A dif1 # diff à la moyenne
declare -A dif2 # diff à la moyenne au carré
declare -A sum2 # calcul de la variance
declare -A avg2 # moyenne
declare -A sdev # ecart type

cp occurence1TagList.txt occurence1TagListSansLangue

sed -i '/^(/d' occurence1TagListSansLangue
sed -i 's/^\ \ *//' occurence1TagListSansLangue
sed -i '0,/^\ /s///' occurence1TagListSansLangue

grep . occurence1TagListSansLangue | cut -d" " -f 1 > valeurs2
grep . occurence1TagListSansLangue | cut -d" " -f 2 > types2

taille=`wc -l valeurs2 | cut -d" " -f 1`

j=1
for j in $(seq 1 $taille) ; do
	valeur=`sed -n "$j p" valeurs2`
	type=`sed -n "$j p" types2`
	if [ ! $valeur ] ; then
		echo pas de valeur
		break
	fi
	if [ ! $type ] ; then
		echo "pas de type"
		break
	fi
	
	# calcul de dif1
	echo ${dif1["$type"]} = ${moyenne["$type"]} - $valeur
	dif1["$type"]=$(echo "scale=$SC; ${moyenne["$type"]} - $valeur" | bc) # Différence avec la moyenne
	echo ${dif1["$type"]}
	if [ ! ${dif1["$type"]} ] ; then
		echo pas de dif1 pour $type
		break
	fi
	
	# calcul de dif2
	echo ${dif1["$type"]} "*" ${dif1["$type"]}
	dif2["$type"]=$(echo "scale=$SC; ${dif1["$type"]} * ${dif1["$type"]}" | bc) # au carré
	echo = ${dif2["$type"]}
	if [ ! ${dif2["$type"]} ] ; then
		echo pas de dif2 pour $type
		break
	fi
	
	#caclul de sum2
	if [ ! ${sum2["$type"]} ] ; then
		sum2["$type"]=0
		echo pas de sum2 pour $type
	fi
	echo ${sum2["$type"]} + ${dif2["$type"]}
	sum2["$type"]=$(echo "scale=$SC; ${sum2["$type"]} + ${dif2["$type"]}" | bc) # somme
	echo = ${sum2["$type"]}
done

echo je suis sorti

exit 1

echo sum2[*] = ${sum2[*]}

${sum2[*]} | tr " " "\n" > var
for ty in var
do
	avg2["$ty"]=$(echo "scale=$SC; ${sum2["$ty"]} / $n" | bc)  # moyenne des écarts
	sdev["$ty"]=$(echo "scale=$SC; sqrt(${avg2["$ty"]})" | bc) # racine carré
done

echo ${sdev[*]}
echo ${sdev[*]} > tableauDesEcartsTypes

