#!/bin/bash

mkdir -p "results"

declare -a corpora=(
    "canterbury"
    "artificial"
    "large"
)

declare -a coders=(
    "lz78-naive"
    "lz78-optimized"
    "lzw-naive"
    "lzw-optimized"
    "compress"
)

declare -a scripts=(
    "./bench-lz78-n.py"
    "./bench-lz78-o.py"
    "./bench-lzw-n.py"
    "./bench-lzw-o.py"
    "./bench-compress.py"
)

pilot="bench run_program --pi \"duration,secs,0,0,1:throughput,MB/s,1,1,1\" --"

for corpus in "${corpora[@]}"
do

    for text in "corpora/${corpus}"/*
    do
        for (( i = 0; i < ${#coders[@]}; i++ ))
        do
            coder=${coders[i]}
            script=${scripts[i]}

            mkdir -p "results/${corpus}/${coder}/encode"
            mkdir -p "results/${corpus}/${coder}/decode"

            encoded="results/${corpus}/${coder}/encode/${text##*/}.summary"
            encmd="${pilot} ${script} -c -i ${text} | tail -n 27 > ${encoded}"

            decoded="results/${corpus}/${coder}/decode/${text##*/}.summary"
            decmd="${pilot} ${script} -d -i ${text} | tail -n 27 > ${decoded}"

            echo -n "Benchmarking ${coder} on ${text##*/}... "
            eval ${encmd}
            eval ${decmd}
            echo "done."
        done
    done
done
