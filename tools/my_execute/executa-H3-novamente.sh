#!/bin/bash

DIRECTORY="batteries"
PATTERN="ct-{BA,BI,ER,WA}[0-9]{1,4}-H3v[1-2].yaml"
#PATTERN="ct-BA*.yaml"

for file in "$DIRECTORY"/$PATTERN; do
    if [[ -f $file ]]; then
        python3 execution-battery.py 1 < "$file"
    else
        echo "O arquivo $file não existe ou não é um arquivo regular."
    fi
done

