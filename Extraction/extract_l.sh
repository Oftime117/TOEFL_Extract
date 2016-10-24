#!/bin/bash
inFile=$1
outFile=$2
echo grep "(" $inFile | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > $outFile
