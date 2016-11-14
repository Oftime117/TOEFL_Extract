#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
input1=$1 # la liste des groupes des 1 à utiliser genre trainTagList.txt ou list1
input2=$2 # la liste des groupes de 1 à utiliser avec les occurences par texte
output=$3 # le nom du fichier de sortie

########################
# calcul de la moyenne #
########################

# créaction des fichiers des moyennes
if [ ! -f "sommeTag" ] ; then
	sed -i '/^(/d' $input1			# supprime la langue
	sort $input1 | uniq -c | sort -bnr > sommeTag
	sed -i 's/^\ \ *//' sommeTag	# supprime les multiples espaces
	sed -i '0,/^\ /s///' sommeTag	# supprime l'espace en début de ligne
	cp sommeTag sommeTag_temp
	./supprPonctu.sh sommeTag_temp sommeTag
	rm sommeTag_temp
fi

# extraction pour contruire un tableau
grep . sommeTag | cut -d" " -f 1 > valeurs
grep . sommeTag | cut -d" " -f 2 > types

> valeurs-types-moyenneGroupe1.txt

# creation tableau moyenne | valeur
declare -A moyenne
declare -A valeurs

SC=9 # on prend 9 chiffres après la virgule
n=9900 # nombre de ligne dans ..data/train/train.txt
while read newline ;
do
	valeur=`echo "${newline}" | cut -d" " -f 1`
	if [ "$valeur" -ge "9900" ]; then 
		type=`echo "${newline}" | cut -d" " -f 2`
		if [ ! "$type" ] ; then
			echo "pas de type pour la valeur: $valeur"
			continue
		fi
		moyenne["$type"]=$(echo "scale=$SC; $valeur / $n" | bc)
		valeurs["$type"]=$valeur
		echo "moyenne[$type] = $valeur / $n = ${moyenne["$type"]}"
		echo "$valeur $type ${moyenne["$type"]}" >> valeurs-types-moyenneGroupe1.txt
	fi
done < sommeTag

#############################
# calcul de l'écart type	#
#############################

# decalrion des tableaux associatifs
declare -A sum2 # calcul de la variance
declare -A avg2 # moyenne
declare -A sdev # ecart type

if [ ! -f "types-sum2-valeursGroupe1.txt" ] ; then
	
	cp $input2 occurence1TagListSansLangue

	sed -i '/^(/d' occurence1TagListSansLangue 			# supprime la langue
	sed -i 's/^\ \ *//' occurence1TagListSansLangue 	# supprime les multiples espaces
	sed -i '0,/^\ /s///' occurence1TagListSansLangue 	# supprime l'espace en début de ligne
	sed -i '/^$/d' occurence1TagListSansLangue 			# supprime les lignes vides

	> types-sum2-valeursGroupe1_temp.txt

	echo "Lancement du calcul de la somme des carrés des différences" 
	while read newline ;
	do
			
			type=`echo "${newline}" | cut -d" " -f 2`
			if [ ${moyenne["$type"]} ]; then
				valeur=`echo "${newline}" | cut -d" " -f 1`
				if [ ! $valeur ] ; then
					if [ $valeur=="$" ] ; then
						echo "fin du fichier"
						break
					else
						echo "pas de valeur pour type: $type"
						break
					fi
				fi
				if [ ! "$type" ] ; then
					echo "pas de type pour valeur: $valeur"
					break
				fi
	
				# calcul de sum2
				if [ ! ${sum2["$type"]} ] ; then
					sum2["$type"]=0 # initialisation de sum2
					echo "$type ${sum2["$type"]}" >> types-sum2-valeursGroupe1_temp.txt
					echo "ajout du type: $type"
				fi
				sum2["$type"]=$(echo "scale=$SC; ${sum2["$type"]} + (${moyenne["$type"]} - $valeur) * (${moyenne["$type"]} - $valeur)" | bc)
				echo "$type ${sum2["$type"]}" >> types-sum2-valeursGroupe1_temp.txt
			fi
			
	done < occurence1TagListSansLangue

	echo "Fin du calcul de la somme des carrés des différences"
	
	echo "types-sum2-valeursGroupe1.txt:"
	# création du fichier qui contient tabel | CarresDesDifferences pour ce label
	> types-sum2-valeursGroupe1.txt
	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 2`
		echo "$type ${sum2["$type"]} ${valeurs["$type"]}"
		echo "$type ${sum2["$type"]} ${valeurs["$type"]}" >> types-sum2-valeursGroupe1.txt
	done < valeurs-types-moyenneGroupe1.txt
elif [ -f "types-sum2-valeursGroupe1.txt" ]; then

########################################
# parser le fichier s'il est déjà créé #
########################################

	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 1`
		valeur=`echo "${newline}" | cut -d" " -f 2`
		nbOcc=`echo "${newline}" | cut -d" " -f 3`
		if [ ! $valeur ] ; then
			if [ "$valeur"=="$" ] ; then
				echo "fin du fichier"
				break
			else
				echo "pas de valeur"
				break
			fi	
		fi
		if [ ! "$type" ] ; then
			echo "pas de type"
			break
		fi
		sum2["$type"]=$valeur
		valeurs["$type"]=$nbOcc
	done < types-sum2-valeursGroupe1.txt
fi

# fichiers de sortie
> $output # types-sdevGroupe1

# calcul à partir de la somme des carrés des differences 
echo "Lancement de la moyenne des ecarts et de la mise à la racine carré"


echo "tableauDesEcartsTypes:"
while read ty ;
do
	# calcul de l'ecart type
	
	sdev["$ty"]=$(echo "scale=$SC; sqrt(${sum2["$ty"]} / $n )" | bc) # racine carré de la moyenne

	echo "sqrt( sum2[$ty] / n) = sqrt( ${sum2["$ty"]} / $n ) = ${sdev["$ty"]} < ${moyenne["$ty"]}"

	# création du fichier à lire en c++
	echo "$ty ${sdev["$ty"]}" >> $output # types-sdevGroupe1
done < types

echo "Le programme a terminé"




