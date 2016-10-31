#!/bin/bash
#Florian TALOUR & Tristan LE NAIR


#utilisation de wapiti
grep "(" data/train.txt | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > tempo.txt

#lancement des scripts comptant les occurences de types
groupe1.sh tempo.txt sortie1.txt
groupe2.sh tempo.txt sortie2.txt
groupe3.sh tempo.txt sortie3.txt
