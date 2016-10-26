#!/bin/bash
let "numline = 1"
grep "(" $1 | cut -d " " -f -1 > listLanguage

> $2

while read newline ;
do

	sed -n "$numline p" listLanguage >> $2
	let "numline = numline + 1"
	echo $newline | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > temp.txt
	sort temp.txt | uniq -c | sort -bnr >> $2
	echo "" >> $2

done < $1

