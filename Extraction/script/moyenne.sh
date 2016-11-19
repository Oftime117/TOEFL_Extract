#!/bin/bash
# Florian TALOUR & Tristan LE NAIR

########################
# calcul de la moyenne #
########################

if [ ! -f "trainTagList.txt" ] ; then
	./extract_l.sh ../data/train.txt trainTagList.txt
fi

# créaction des fichiers des moyennes
if [ ! -f "listeMoyenne" ] ; then
	sort trainTagList.txt | uniq -c | sort -bnr > listeMoyenne
	sed -i 's/^\ \ *//' listeMoyenne	# supprime les multiples espaces
	sed -i '0,/^\ /s///' listeMoyenne	# supprime l'espace en début de ligne
	sed -i '/^9900/d' listeMoyenne		# supprime le nombre de \n
fi

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
	#echo $valeur $type ${moyenne[$type]}
	let "i = i + 1"
done < listeMoyenne

#echo ${moyenne[*]}

#############################
# calcul de l'écart type	#
#############################

# decalrion des tableaux associatifs
declare -A sum2 # calcul de la variance
declare -A avg2 # moyenne
declare -A sdev # ecart type
	
if [ ! -f "sommeDesCarresDesDifferencesList.txt" ] ; then
	
	cp occurence1TagList.txt occurence1TagListSansLangue

	sed -i '/^(/d' occurence1TagListSansLangue 			# supprime la langue
	sed -i 's/^\ \ *//' occurence1TagListSansLangue 	# supprime les multiples espaces
	sed -i '0,/^\ /s///' occurence1TagListSansLangue 	# supprime l'espace en début de ligne
	sed -i '/^$/d' occurence1TagListSansLangue 			# supprime les lignes vides

	echo "Lancement du calcul de la somme des carrés des différences" 
	while read newline ;
	do
			valeur=`echo "${newline}" | cut -d" " -f 1` #`sed -n "$j p" valeurs2`
			type=`echo "${newline}" | cut -d" " -f 2` #`sed -n "$j p" types2`
		
			if [ ! $valeur ] ; then
				if [ $valeur=="$" ] ; then
					echo "fin du fichier"
					break
				else
					echo "pas de valeur"
					break
				fi	
			fi
			if [ ! $type ] ; then
				echo "pas de type"
				break
			fi
	
			# caclul de sum2
			if [ ! ${sum2["$type"]} ] ; then
				sum2["$type"]=0 # initialisation de sum2
				echo pas de sum2 pour $type
			fi
			sum2["$type"]=$(echo "scale=$SC; ${sum2["$type"]} + (${moyenne["$type"]} - $valeur) * (${moyenne["$type"]} - $valeur)" | bc)
	done < occurence1TagListSansLangue

	echo "Fin du calcul de la somme des carrés des différences"

	echo "sum2[*] =" ${sum2[*]}

	echo "Lancement de la moyenne des ecarts et de la mise à la racine carré"
	echo ${sum2[*]} | tr " " "\n" > sommeDesCarresDesDifferencesList.txt
	
	# création du fichier qui contient tabel | CarresDesDifferences pour ce label
	> tagCarreMoyenneEcart
	i=1
	while read ty ;
	do
		echo $ty `sed -n "$i p" sommeDesCarresDesDifferencesList.txt` >> tagCarreMoyenneEcart
		let "i = i + 1"
	done < types
fi

########################################
# parser le fichier s'il est déjà créé #
########################################
if [ -f "tagCarreMoyenneEcart" ]; then
	while read newline ;
	do
		type=`echo "${newline}" | cut -d" " -f 1` #`sed -n "$i p" typeSommeDesCarresDesDifferences.txt`
		valeur=`echo "${newline}" | cut -d" " -f 2` #`sed -n "$i p" sommeDesCarresDesDifferencesList.txt`
		if [ ! $valeur ] ; then
			if [ $valeur=="$" ] ; then
				echo "fin du fichier"
				break
			else
				echo "pas de valeur"
				break
			fi	
		fi
		if [ ! $type ] ; then
			echo "pas de type"
			break
		fi
		sum2["$type"]=$valeur
	done < tagCarreMoyenneEcart
fi

# calcul à partir de la somme des carrés des differences 
while read ty ;
do
	sdev["$ty"]=$(echo "scale=$SC; sqrt(${sum2["$ty"]} / $n)" | bc) # racine carré
done < types

echo ${sdev[*]}
echo ${sdev[*]} | tr " " "\n" > tableauDesEcartsTypes

# fichier de sortie
> fichierTypeEcartType

# création du fichier à lire en c++
i=1
while read ty ;
do
	ecartType=`sed -n "$i p" tableauDesEcartsTypes`
	let "i = i + 1"
	echo "$ty $ecartType" >> fichierTypeEcartType
done < types




