#!/bin/bash

DIRECTORY="batteries"
PATTERN="grafo15-41-bf-*.yaml"

for file in "$DIRECTORY"/$PATTERN; do
    if [[ -f $file ]]; then
        python3 execution-battery.py 1 < "$file"
    else
        echo "O arquivo $file não existe ou não é um arquivo regular."
    fi
done
