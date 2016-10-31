#!/bin/bash
#Florian TALOUR & Tristan LE NAIR
grep "(" $1 | cut -d " " -f 2- > $2
