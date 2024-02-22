#!/bin/bash
for prefix in BA BI ER WA
do
    for year in $(seq 500 1000)
    do
        for z in 1 2
        do
            file="batteries/ct-${prefix}${year}-H3v${z}.yaml"
            if [[ -f $file ]]; then
                nohup python3 execution-battery.py 1 < $file
            fi
        done
    done
done

