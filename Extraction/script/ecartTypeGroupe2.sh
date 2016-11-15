#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
#input1=$1 # la liste des groupes des 2 à utiliser
#input2=$2 # la liste des groupes des 2 à utiliser avec les occurences par texte
#output=$3 # le nom du fichier de sortie

########################
# calcul de la moyenne #
########################

if [ ! -f "trainTagList.txt" ] ; then
	echo ">> Fabrication des labels"
	./extract_l.sh ../data/train.txt trainTagList.txt
fi

if [ ! -f "occurence2TagList.txt" ] ; then
	echo ">> Lancement du calcul des occurences par texte pour deux labels consécutifs"
	./groupe2.sh trainTagList.txt occurence2TagList.txt &
fi

cp list2.txt list2SansLangue.txt
# créaction des fichiers des moyennes
if [ ! -f "sommeTagGroupe2" ] ; then
	sed -i '/^(/d' list2SansLangue.txt		# supprime la langue
	sort list2SansLangue.txt | uniq -c | sort -bnr > sommeTagGroupe2
	sed -i 's/^\ \ *//' sommeTagGroupe2		# supprime les multiples espaces
	sed -i '0,/^\ /s///' sommeTagGroupe2	# supprime l'espace en début de ligne
	sed -i '/^$/d' sommeTagGroupe2 			# supprime les lignes vides
	sed -i 's/\ /_/2' sommeTagGroupe2
	cp sommeTagGroupe2 listeMoyenne_temp
	./supprPonctu.sh listeMoyenne_temp sommeTagGroupe2
	rm listeMoyenne_temp
fi

# extraction pour contruire un tableau
grep . sommeTagGroupe2 | cut -d" " -f 1 > valeursGroupe2
grep . sommeTagGroupe2 | cut -d" " -f 2 > typesGroupe2

# creation tableau moyenne | valeur
declare -A moyenne
declare -A valeurs

> valeurs-types-moyenneGroupe2.txt

SC=9 # on prend 9 chiffres après la virgule
n=9900 # nombre de ligne dans ..data/train/train.txt
while read newline ;
do
	valeur=`echo "${newline}" | cut -d" " -f 1`
	type=`echo "${newline}" | cut -d" " -f 2`
	if [ ! "$type" ] ; then
		echo "pas de type pour la valeur: $valeur"
		continue
	fi
	moyenne["$type"]=$(echo "scale=$SC; $valeur / $n" | bc)
	valeurs["$type"]=$valeur
	echo "$valeur $type ${moyenne["$type"]}" >> valeurs-types-moyenneGroupe2.txt
done < sommeTagGroupe2

# affichage des moyennes et supression des types qui ne sont pas dans moyenne
cp typesGroupe2 types_temp

> typesGroupe2
> moyennesGroupe2

echo "calcul des moyennes:"
while read ty ;
do
	if [ $ty ]; then # suppression des types incorrects
		echo "$ty" >> typesGroupe2
		echo "$ty ${moyenne["$ty"]}" >> moyennesGroupe2
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
	
if [ ! -f "types-sum2-valeursGroupe2.txt" ] ; then
	
	cp occurence2TagList.txt occurence2TagListSansLangue

	sed -i '/^(/d' occurence2TagListSansLangue 			# supprime la langue
	sed -i 's/^\ \ *//' occurence2TagListSansLangue 	# supprime les multiples espaces
	sed -i '0,/^\ /s///' occurence2TagListSansLangue 	# supprime l'espace en début de ligne
	sed -i '/^$/d' occurence2TagListSansLangue 			# supprime les lignes vides
	sed -i 's/\ /_/2' occurence2TagListSansLangue

	echo "Lancement du calcul de la somme des carrés des différences" 
	while read newline ;
	do
			valeur=`echo "${newline}" | cut -d" " -f 1`
			type=`echo "${newline}" | cut -d" " -f 2`
		
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
	done < occurence2TagListSansLangue

	echo "Fin du calcul de la somme des carrés des différences"
	
	# création du fichier qui contient tabel | CarresDesDifferences pour ce label
	> types-sum2-valeursGroupe2.txt
	while read ty ;
	do
		echo "$ty ${sum2["$ty"]} ${valeurs["$ty"]}" >> types-sum2-valeursGroupe2.txt
	done < typesGroupe2
fi

########################################
# parser le fichier s'il est déjà créé #
########################################
if [ -f "types-sum2-valeursGroupe2.txt" ]; then
	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 1`
		valeur=`echo "${newline}" | cut -d" " -f 2`
		nbOcc=`echo "${newline}" | cut -d" " -f 3`
		if [ ! $valeur ] ; then
			if [ $valeur=="$" ] ; then
				echo "fin du fichier"
				break
			else
				echo "pas de valeur pour type: $type"
				break
			fi	
		fi
		if [ ! $type ] ; then
			echo "pas de type pour valeur: $valeur"
			break
		fi
		sum2["$type"]=$valeur
		valeurs["$type"]=$nbOcc
	done < types-sum2-valeursGroupe2.txt
fi

# fichiers de sortie
> types-sdevGroupe2

# calcul à partir de la somme des carrés des differences 
echo "Lancement de la moyenne des ecarts et de la mise à la racine carré"

echo "tableauDesEcartsTypes:"
while read ty ;
do
	# calcul de l'ecart type
	sdev["$ty"]=$(echo "scale=$SC; sqrt(${sum2["$ty"]} / $n)" | bc) # racine carré de la moyenne

	echo "$ty ${sum2["$ty"]} ${sdev["$ty"]} ${moyenne["$ty"]}"

	# création du fichier à lire en c++
	echo "$ty ${sdev["$ty"]}" >> types-sdevGroupe2
done < typesGroupe2

echo "Le programme a terminé"




