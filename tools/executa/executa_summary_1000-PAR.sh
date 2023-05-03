#!/bin/bash

for i in ../workspace/ct-BA1000-PAR-09042023-01 ; do
  if [ -d "$i" ]; then
    echo "$i"
    python3 execution-battery-analyzer.py $i
  fi
done

for i in ../workspace/ct-BI1000-PAR-09042023-01 ; do
  if [ -d "$i" ]; then
    echo "$i"
    python3 execution-battery-analyzer.py $i
  fi
done

for i in ../workspace/ct-ER1000-PAR-10042023-01 ; do
  if [ -d "$i" ]; then
    echo "$i"
    python3 execution-battery-analyzer.py $i
  fi
done

for i in ../workspace/ct-WA1000-PAR-10042023-01 ; do
  if [ -d "$i" ]; then
    echo "$i"
    python3 execution-battery-analyzer.py $i
  fi
done