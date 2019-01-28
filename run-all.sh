#!/usr/bin/env bash

set -e

for f in */*; do
    if [ -d ${f} ]; then
        # Will not run if no directories are available
        if ! [[ ${f} =~ ^cmake* ]] && ! [[ ${f} =~ ^extra* ]] && ! [[ ${f} =~ ^main.* ]]; then
            echo "[$f] Testing";
            sh run.sh $f;
        fi
    fi
done