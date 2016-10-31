#!/bin/bash
#Tristan LE NAIR

echo $1 | read newline
echo $newline | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > temp.txt
sort temp.txt | uniq -c | sort -bnr > $2

while read newline ;
do

	echo $newline | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > temp.txt
	sort temp.txt | uniq -c | sort -bnr >> $2
	echo "" >> $2

done < $1

#tr [:space:] '\n' | grep -v "^\s*$" | sort | uniq -c | sort -bnr
#ou
#for w in `cat trainwap`; do echo $w; done|sort|uniq -c

