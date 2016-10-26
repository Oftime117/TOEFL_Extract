grep "(" $1 | cut -d ")" -f 2- | tr " " "\n" | wapiti-1.5.0/wapiti label -l -m model-pos.en > $2
