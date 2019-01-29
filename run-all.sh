#!/usr/bin/env bash

set -e
set -x

g++ -std=c++11 -lCGAL -lmpfr -lgmp -lCGAL_Core -I /usr/local/opt/boost/include/boost/ -Wall -g -O3 week10/evolution/main.cpp -o main
bash main

for f in */*; do
    if [ -d ${f} ]; then
        # Will not run if no directories are available
        if ! [[ ${f} =~ ^cmake* ]] && ! [[ ${f} =~ ^extra* ]] && ! [[ ${f} =~ ^main.* ]]; then
            echo "[$f] Testing";
            sh run.sh $f;
        fi
    fi
done