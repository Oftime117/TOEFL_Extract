#!/bin/bash
# Florian TALOUR & Tristan LE NAIR

########################
# calcul de la moyenne #
########################

if [ ! -f "trainTagList.txt" ] ; then
	echo ">> Fabrication des labels"
	./extract_l.sh ../data/train.txt trainTagList.txt
fi

if [ ! -f "occurence3TagList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
	./groupe3.sh trainTagList.txt occurence3TagList.txt &
fi

cp list3.txt list3SansLangue.txt
# créaction des fichiers des moyennes
if [ ! -f "sommeTagGroupe3" ] ; then
	sed -i '/^(/d' list3SansLangue.txt			# supprime la langue
	sort list3SansLangue.txt | uniq -c | sort -bnr > sommeTagGroupe3
	sed -i 's/^\ \ *//' sommeTagGroupe3		# supprime les multiples espaces
	sed -i '0,/^\ /s///' sommeTagGroupe3	# supprime l'espace en début de ligne
	sed -i '/^$/d' sommeTagGroupe3 			# supprime les lignes vides
	sed -i 's/\ /_/2' sommeTagGroupe3
	sed -i 's/\ /_/2' sommeTagGroupe3
	cp sommeTagGroupe3 listeMoyenne_temp
	./supprPonctu.sh listeMoyenne_temp sommeTagGroupe3
	rm listeMoyenne_temp
fi

# extraction pour contruire un tableau
grep . sommeTagGroupe3 | cut -d" " -f 1 > valeursGroupe3
grep . sommeTagGroupe3 | cut -d" " -f 2 > typesGroupe3

# creation tableau moyenne | valeur
declare -A moyenne

SC=9 # on prend 9 chiffres après la virgule
n=9900 # nombre de ligne dans ..data/train/train.txt
valeurMoyenne=0
while read newline ;
do
	valeur=`echo "${newline}" | cut -d" " -f 1`
	type=`echo "${newline}" | cut -d" " -f 2`
	if [ ! "$type" ] ; then
		echo "pas de type pour la valeur: $valeur"
		continue
	fi
	moyenne["$type"]=$(echo "scale=$SC; $valeur / $n" | bc)
done < sommeTagGroupe3

# affichage des moyennes et supression des types qui ne sont pas dans moyenne
cp typesGroupe3 types_temp

> typesGroupe3
> moyennesGroupe3

echo "calcul des moyennes:"
while read ty ;
do
	if [ $ty ]; then # suppression des types incorrects
		echo "$ty" >> typesGroupe3
		echo "$ty ${moyenne[$ty]}" >> moyennesGroupe3
	fi
done < types_temp

rm types_temp

#############################
# calcul de l'écart type	#
#############################

# decalrion des tableaux associatifs
declare -A sum2 # calcul de la variance
declare -A avg2 # moyenne
declare -A sdev # ecart type
	
if [ ! -f "tagsommeDesCarresDesDifferencesListGroupe3.txt" ] ; then
	
	cp occurence3TagList.txt occurence3TagListSansLangue

	sed -i '/^(/d' occurence3TagListSansLangue 			# supprime la langue
	sed -i 's/^\ \ *//' occurence3TagListSansLangue 	# supprime les multiples espaces
	sed -i '0,/^\ /s///' occurence3TagListSansLangue 	# supprime l'espace en début de ligne
	sed -i '/^$/d' occurence3TagListSansLangue 			# supprime les lignes vides
	sed -i 's/\ /_/2' occurence3TagListSansLangue
	sed -i 's/\ /_/2' occurence3TagListSansLangue

	echo "Lancement du calcul de la somme des carrés des différences" 
	while read newline ;
	do
			valeur=`echo "${newline}" | cut -d" " -f 1` #`sed -n "$j p" valeurs2`
			type=`echo "${newline}" | cut -d" " -f 2-3` #`sed -n "$j p" types2`
		
			if [ ! $valeur ] ; then
				if [ "$valeur"=="$" ] ; then
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
	done < occurence3TagListSansLangue

	echo "Fin du calcul de la somme des carrés des différences"
	
	# création du fichier qui contient tabel | CarresDesDifferences pour ce label
	> tagsommeDesCarresDesDifferencesListGroupe3.txt
	while read ty ;
	do
		echo "$ty ${sum2["$ty"]} ${occ["$type"]}" >> tagsommeDesCarresDesDifferencesListGroupe3.txt
	done < typesGroupe3
fi

########################################
# parser le fichier s'il est déjà créé #
########################################
if [ -f "tagsommeDesCarresDesDifferencesListGroupe3.txt" ]; then
	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 1-2`
		valeur=`echo "${newline}" | cut -d" " -f 3`
		if [ ! $valeur ] ; then
			if [ "$valeur"=="$" ] ; then
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
		sum2["$type"]=$valeur
	done < tagsommeDesCarresDesDifferencesListGroupe3.txt
fi

# fichiers de sortie
> fichierTypeEcartTypeGroupe3

# calcul à partir de la somme des carrés des differences 
echo "Lancement de la moyenne des ecarts et de la mise à la racine carré"

echo "tableauDesEcartsTypes:"
while read ty ;
do
	# calcul de l'ecart type
	sdev["$ty"]=$(echo "scale=$SC; sqrt(${sum2["$ty"]} / ${occ["$type"])})" | bc) # racine carré de la moyenne
	
	echo "sqrt( sum2[$ty] / n) = sqrt( ${sum2["$ty"]} / ${occ["$type"])} ) = ${sdev["$ty"]} < ${moyenne["$ty"]}"

	# création du fichier à lire en c++
	echo "$ty ${sdev["$ty"]}" >> fichierTypeEcartTypeGroupe3
done < typesGroupe3

echo "Le programme a terminé"




