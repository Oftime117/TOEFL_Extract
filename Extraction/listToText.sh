#!/bin/bash
#Florian TALOUR & Tristan LE NAIR
cat $1 | tr "\n" " " | tr "[:space:]{2}" "\n" > $2
