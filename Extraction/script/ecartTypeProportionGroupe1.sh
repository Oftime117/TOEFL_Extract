#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
#input1=$1 # la liste des groupes des 1 à utiliser genre trainMotList.txt ou list1
#input2=$2 # la liste des groupes de 1 à utiliser avec les occurences par texte
#output=$3 # le nom du fichier de sortie

if [ "$1" -eq "1" ]; then # supprimer tout
	rm sommeMot.txt valeurs-mots-moyenneGroupe1.txt valeurs.txt types.txt mots-sum2-valeursGroupe1.txt proportion1MotListSansLangue.txt mots-sum2-valeursGroupe1_temp.txt 
elif [ "$1" -eq "2" ]; then # supprimer tout
	rm valeurs-mots-moyenneGroupe1.txt valeurs.txt types.txt mots-sum2-valeursGroupe1.txt proportion1MotListSansLangue.txt mots-sum2-valeursGroupe1_temp.txt 
fi


########################
# calcul de la moyenne #
########################

# créaction des fichiers des moyennes
if [ ! -f "sommeMot.txt" ] ; then
	sed -i '/^(/d' trainMotList.txt	# supprime la langue
	sort trainMotList.txt | uniq -c | sort -bnr > sommeMot.txt
	sed -i 's/^\ \ *//' sommeMot.txt	# supprime les multiples espaces
	sed -i '0,/^\ /s///' sommeMot.txt	# supprime l'espace en début de ligne
	
	cp sommeMot.txt sommeMot_temp.txt
	./supprPonctu.sh sommeMot_temp.txt sommeMot.txt
	rm sommeMot_temp.txt
fi

# extraction pour contruire un tableau
grep . sommeMot.txt | cut -d" " -f 1 > valeurs.txt
grep . sommeMot.txt | cut -d" " -f 2 > types.txt

# creation tableau moyenne | valeur
declare -A moyenne
declare -A valeurs

SC=9 # on prend 9 chiffres après la virgule
n=`cat trainMotList.txt | sed "/^$/d" | wc -l` # nombre total de mots dans trainMotList.txt

if [ ! -f "valeurs-mots-moyenneGroupe1.txt" ] ; then
	> valeurs-mots-moyenneGroupe1.txt

	while read newline ;
	do
		valeur=`echo "${newline}" | cut -d" " -f 1`
		if [ "$valeur" -gt "9900" ]; then 
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
	done < sommeMot.txt
elif [ -f "valeurs-mots-moyenneGroupe1.txt" ] ; then
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

	cp proportion1MotList.txt proportion1MotListSansLangue.txt

	sed -i '/^(/d' proportion1MotListSansLangue.txt 			# supprime la langue
	sed -i 's/^\ \ *//' proportion1MotListSansLangue.txt 	# supprime les multiples espaces
	sed -i '0,/^\ /s///' proportion1MotListSansLangue.txt 	# supprime l'espace en début de ligne
	sed -i '/^$/d' proportion1MotListSansLangue.txt 			# supprime les lignes vides
	
	> proportion1MotListSansLangue_temp.txt
	
	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 2`
		grep " $type$" proportion1MotListSansLangue.txt >> proportion1MotListSansLangue_temp.txt
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

	done < proportion1MotListSansLangue_temp.txt

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
	
	# rm proportion1MotListSansLangue_temp.txt
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
> mots-sdevGroupe1.txt

# calcul à partir de la somme des carrés des differences 
echo "Lancement de la moyenne des ecarts et de la mise à la racine carré"


echo "tableauDesEcartsTypes:"
while read newline ;
do
	type=`echo "${newline}" | cut -d" " -f 2`
	
	# calcul de l'ecart type
	sdev["$type"]=$(echo "scale=$SC; sqrt(${sum2["$type"]} / 9900 )" | bc) # racine carré de la moyenne

	echo "sqrt( sum2[$type] / n) = sqrt( ${sum2["$type"]} / 9900 ) = ${sdev["$type"]} < ${moyenne["$type"]}"

	# création du fichier à lire en c++
	echo "$type ${sdev["$type"]}" >> mots-sdevGroupe1.txt
	
done < valeurs-mots-moyenneGroupe1.txt

echo "Le programme a terminé"
