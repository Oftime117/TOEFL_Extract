#!/bin/bash
#Florian TALOUR & Tristan LE NAIR
sed -i "s/\ \ */\ /g" $1 
cat $1 | tr "\n" " " | tr "(" "\n" > $2
sed -i '1d' $2
sed -i 's/^/(/' $2
