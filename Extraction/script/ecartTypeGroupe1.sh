#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
#input1=$1 # la liste des groupes des 1 à utiliser genre trainTagList.txt ou list1
#input2=$2 # la liste des groupes de 1 à utiliser avec les occurences par texte
#output=$3 # le nom du fichier de sortie

########################
# calcul de la moyenne #
########################

# créaction des fichiers des moyennes
if [ ! -f "sommeMot" ] ; then
	sed -i '/^(/d' trainMotListLower.txt	# supprime la langue
	sort trainMotListLower.txt | uniq -c | sort -bnr > sommeMot
	sed -i 's/^\ \ *//' sommeMot	# supprime les multiples espaces
	sed -i '0,/^\ /s///' sommeMot	# supprime l'espace en début de ligne
	
	cp sommeMot sommeMot_temp
	./supprPonctu.sh sommeMot_temp sommeMot
	rm sommeMot_temp
fi

# extraction pour contruire un tableau
grep . sommeMot | cut -d" " -f 1 > valeurs
grep . sommeMot | cut -d" " -f 2 > types

# creation tableau moyenne | valeur
declare -A moyenne
declare -A valeurs

SC=9 # on prend 9 chiffres après la virgule
n=9900 # nombre de ligne dans ..data/train/train.txt

if [ ! -f valeurs-mots-moyenneGroupe1.txt ] ; then
	> valeurs-mots-moyenneGroupe1.txt

	while read newline ;
	do
		valeur=`echo "${newline}" | cut -d" " -f 1`
		if [ "$valeur" -ge "$n" ]; then 
			type=`echo "${newline}" | cut -d" " -f 2`
			if [ ! "$type" ] ; then
				echo "pas de type pour la valeur: $valeur"
				continue
			fi
			moyenne["$type"]=$(echo "scale=$SC; $valeur / $n" | bc)
			valeurs["$type"]=$valeur
			echo "moyenne[$type] = $valeur / $n = ${moyenne["$type"]}"
			echo "$valeur $type ${moyenne["$type"]}" >> valeurs-mots-moyenneGroupe1.txt
		fi
	done < sommeMot
elif [ -f valeurs-mots-moyenneGroupe1.txt ] ; then
	while read newline ;
	do
		valeur=`echo "${newline}" | cut -d" " -f 1`
		type=`echo "${newline}" | cut -d" " -f 2`
		moyenne=`echo "${newline}" | cut -d" " -f 3`
		moyenne["$type"]=$moyenne
		valeurs["$type"]=$valeur
	done < valeurs-mots-moyenneGroupe1.txt
fi

#############################
# calcul de l'écart type	#
#############################

# decalrion des tableaux associatifs
declare -A sum2 # calcul de la variance
declare -A avg2 # moyenne
declare -A sdev # ecart type

if [ ! -f "mots-sum2-valeursGroupe1.txt" ] ; then

	cp occurence1MotListLower.txt occurence1MotListSansLangue

	sed -i '/^(/d' occurence1MotListSansLangue 			# supprime la langue
	sed -i 's/^\ \ *//' occurence1MotListSansLangue 	# supprime les multiples espaces
	sed -i '0,/^\ /s///' occurence1MotListSansLangue 	# supprime l'espace en début de ligne
	sed -i '/^$/d' occurence1MotListSansLangue 			# supprime les lignes vides
	
	> occurence1MotListSansLangue_temp
	
	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 2`
		grep " $type$" occurence1MotListSansLangue >> occurence1MotListSansLangue_temp
	done < valeurs-mots-moyenneGroupe1.txt

	> mots-sum2-valeursGroupe1_temp.txt

	echo "Lancement du calcul de la somme des carrés des différences" 
	while read newline ;
	do
			type=`echo "${newline}" | cut -d" " -f 2`
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
				echo "ajout du type: $type"
			fi
			sum2["$type"]=$(echo "scale=$SC; ${sum2["$type"]} + (${moyenne["$type"]} - $valeur) * (${moyenne["$type"]} - $valeur)" | bc)
			echo "$type ${sum2["$type"]} $valeur" >> mots-sum2-valeursGroupe1_temp.txt

	done < occurence1MotListSansLangue_temp

	echo "Fin du calcul de la somme des carrés des différences"
	
	echo "mots-sum2-valeursGroupe1.txt:"
	# création du fichier qui contient tabel | CarresDesDifferences pour ce label
	> mots-sum2-valeursGroupe1.txt
	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 2`
		echo "$type ${sum2["$type"]} ${valeurs["$type"]}"
		echo "$type ${sum2["$type"]} ${valeurs["$type"]}" >> mots-sum2-valeursGroupe1.txt
	done < valeurs-mots-moyenneGroupe1.txt
	
	# rm occurence1MotListSansLangue_temp
elif [ -f "mots-sum2-valeursGroupe1.txt" ]; then

########################################
# parser le fichier s'il est déjà créé #
########################################

	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 1`
		sum2=`echo "${newline}" | cut -d" " -f 2`
		valeur=`echo "${newline}" | cut -d" " -f 3`
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
		sum2["$type"]=$sum2
	done < mots-sum2-valeursGroupe1.txt
fi

# fichiers de sortie
> mots-sdevGroupe1

# calcul à partir de la somme des carrés des differences 
echo "Lancement de la moyenne des ecarts et de la mise à la racine carré"


echo "tableauDesEcartsTypes:"
while read newline ;
do
	type=`echo "${newline}" | cut -d" " -f 2`
	
	# calcul de l'ecart type
	sdev["$type"]=$(echo "scale=$SC; sqrt(${sum2["$type"]} / ${valeurs["$type"]} )" | bc) # racine carré de la moyenne

	echo "sqrt( sum2[$type] / n) = sqrt( ${sum2["$type"]} / ${valeurs["$type"]} ) = ${sdev["$type"]} < ${moyenne["$type"]}"

	# création du fichier à lire en c++
	echo "$type ${sdev["$type"]}" >> mots-sdevGroupe1
	
done < valeurs-mots-moyenneGroupe1.txt

echo "Le programme a terminé"




