#!/bin/bash
for file in instances/grafos/*
do
    echo "Processing $file"
    ./build/release/app_MAIN -r 1 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 2 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 3 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 4 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 5 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 6 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 7 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 8 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 9 -t 4 -s < $file >> autepute.txt
    ./build/release/app_MAIN -r 10 -t 4 -s < $file >> autepute.txt
done
