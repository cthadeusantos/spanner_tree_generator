#!/bin/bash

for i in ../workspace/ct-hr-* ; do
  if [ -d "$i" ]; then
    echo "$i"
    python3 execution-battery-analyzer.py $i
  fi
done
