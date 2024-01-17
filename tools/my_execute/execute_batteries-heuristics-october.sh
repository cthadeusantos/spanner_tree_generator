#!/bin/sh
for file in batteries/ct-oct-BA*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

for file in batteries/ct-oct-BI*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

for file in batteries/ct-oct-ER*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

for file in batteries/ct-oct-WA*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

