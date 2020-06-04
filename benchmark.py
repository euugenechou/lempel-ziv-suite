#!/usr/local/bin/python3

import os
import argparse
import filecmp
import subprocess as sp

##
## Populating command-line arguments.
##

parser = argparse.ArgumentParser()

parser.add_argument("--artificial", action="store_true")
parser.add_argument("--calgary", action="store_true")
parser.add_argument("--canterbury", action="store_true")
parser.add_argument("--large", action="store_true")
parser.add_argument("--all", action="store_true")

args = parser.parse_args()


##
## Parsing command-line arguments.
##

corpora = []

if args.artificial or args.all:
    corpora.append("artificial")

if args.calgary or args.all:
    corpora.append("calgary")

if args.canterbury or args.all:
    corpora.append("canterbury")

if args.large or args.all:
    corpora.append("large")


##
## For convenience.
##

coders = [
    "lz78-naive",
    "lz78-optimized",
    "lzw-naive",
    "lzw-optimized",
    "compress",
    "gzip"
]

encoders = [f"{c}/encode" for c in coders if c != "compress" and c != "gzip"]
decoders = [f"{c}/decode" for c in coders if c != "compress" and c != "gzip"]


##
## Making sure all encoders and decoders are built.
##

print("Building encoders and decoders...", end=' ')

for coder in coders:
    sp.run(f"make -C {coder}", capture_output=True, shell=True)

print("done.")


##
## Running the tests. Unix compress and gzip are added manually.
##

encoders.append("compress -c")
encoders.append("gzip -f -c")

decoders.append("uncompress -c")
decoders.append("gzip -d -c")

for corpus in corpora:
    print(f"\n  {corpus.capitalize()} Corpus results:")

    corpus = f"corpora/{corpus}"

    for text in os.listdir(corpus):
        uncomp = f"{corpus}/{text}"
        comped = f"/tmp/{text}.zipped"
        decomp = f"/tmp/{text}"

        orig_size = os.stat(uncomp).st_size

        if orig_size > 1:
            print(f"\n    *** {text}: {orig_size} bytes ***")
        else:
            print(f"\n    *** {text}: {orig_size} byte ***")

        for c, e, d in zip(coders, encoders, decoders):
            sp.run(f"{e} < {uncomp} > {comped}", capture_output=True,
                    shell=True)
            sp.run(f"{d} < {comped} > {decomp}", capture_output=True,
                    shell=True)

            assert(filecmp.cmp(uncomp, decomp))

            comp_size = os.stat(comped).st_size
            ratio = 100 * (1 - (comp_size / orig_size))

            print(f"      -- {c}: {comp_size} bytes, ratio: {ratio:.2f}%")


##
## Clean up all encoders and decoders.
##

print("\nCleaning encoders and decoders...", end=' ')

for coder in coders:
    sp.run(f"make clean -C {coder}", capture_output=True, shell=True)

print("done.")
