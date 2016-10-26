#!/bin/bash
#delete all kind of ponctuation in the file
sed -e '/,/d' -e '/\./d' -e "/'/d" -e "/:/d" -e "/``/d" -e "/(/d" -e "/)/d" $1 > $2
