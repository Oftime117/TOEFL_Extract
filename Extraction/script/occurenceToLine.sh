#!/bin/bash
#Florian TALOUR & Tristan LE NAIR
cat $1 | sed '/^$/d' | sed 's/\(^([A-Z]\{3\},\)/§§§§§\1/g' | tr '\n' ' ' | sed 's/§§§§§/\n/g' | sed 's/\ \ */\ /g' > $2
