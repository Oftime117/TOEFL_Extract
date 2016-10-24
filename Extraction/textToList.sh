grep "(" $1 | cut -d ")" -f 2- | tr " " "\n" > $2
