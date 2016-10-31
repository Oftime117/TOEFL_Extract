#!/bin/bash
#Florian TALOUR & Tristan LE NAIR
sort $1 | uniq -c | sort -bnr > $2
