#!/bin/bash

for i in ../workspace/ct-* ; do
  if [ -d "$i" ]; then
    echo "$i"
    python3 execution-battery-analyzer.py $i
  fi
done
