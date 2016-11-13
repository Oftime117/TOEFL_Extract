#!/bin/bash
# Florian TALOUR & Tristan LE NAIR
input=$1
langue=$2
output=$3

cat ${input} | grep -e "($langue," | cut -d " " -f 2- > $output
sed -i 's/\([0-9][0-9]*\)/\n\1/g' $output
sed -i '/^$/d' $output
