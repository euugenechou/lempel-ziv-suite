#!/usr/local/bin/python3

import re
import sys
import os

coders = [
    "lz78-naive",
    "lz78-optimized",
    "lzw-naive",
    "lzw-optimized"
]

if len(sys.argv) != 4:
    print(f"Usage: {sys.argv[0]} [corpus] [mode] [pi]")
    sys.exit(1)

corpus = sys.argv[1]
mode = sys.argv[2]
pi = sys.argv[3]

for file in sorted(os.listdir(f"corpora/{corpus}")):
    print(f"{file}:")

    for coder in coders:
        path = f"results/{corpus}/{coder}/{mode}/{file}.summary"

        with open(path) as summary:
            lines = [line.rstrip('\n') for line in summary]

            if pi == "duration":
                dur_mean = lines[7].split(' ')[-2]
                dur_ci = lines[8].split(' ')[-2]
                dur_var = lines[9].split(' ')[-2]
                print(f"& {dur_mean} & {dur_ci} & {dur_var}")

            if pi == "throughput":
                tp_mean = lines[15].split(' ')[-2]
                tp_ci = lines[16].split(' ')[-2]
                tp_var = lines[17].split(' ')[-2]
                print(f"& {tp_mean} & {tp_ci} & {tp_var}")

    print()
