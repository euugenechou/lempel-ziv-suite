#!/usr/local/bin/python3

import os
import argparse
import filecmp
import time
import subprocess as sp

##
## Populating command-line arguments.
##

parser = argparse.ArgumentParser()

parser.add_argument(
    "--artificial",
    help="Use Artificial Corpus texts",
    action="store_true"
)

parser.add_argument(
    "--calgary",
    help="Use Calgary Corpus texts",
    action="store_true"
)

parser.add_argument(
    "--canterbury",
    help="Use Canterbury Corpus texts",
    action="store_true"
)

parser.add_argument(
    "--large",
    help="Use Large Corpus texts",
    action="store_true"
)

parser.add_argument(
    "--all",
    help="Use all supplied corpora",
    action="store_true"
)

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
            print(f"\n   === {text}: {orig_size} bytes ===")
        else:
            print(f"\n   === {text}: {orig_size} byte ===")

        for c, e, d in zip(coders, encoders, decoders):
            start = time.time()
            sp.run(f"{e} < {uncomp} > {comped}", capture_output=True,
                    shell=True)
            end = time.time()
            time_en = end - start

            start = time.time()
            sp.run(f"{d} < {comped} > {decomp}", capture_output=True,
                    shell=True)
            end = time.time()
            time_de = end - start

            assert(filecmp.cmp(uncomp, decomp))

            comp_size = os.stat(comped).st_size
            ratio = 100 * (1 - (comp_size / orig_size))

            print(f"      {c}:")
            print(f"         Compressed size: {comp_size} bytes")
            print(f"         Compression ratio: {ratio:.2f}%")
            print(f"         Compressed in: {time_en} secs")
            print(f"         Decompressed in: {time_de} secs")


##
## Clean up all encoders and decoders.
##

print("\nCleaning encoders and decoders...", end=' ')

for coder in coders:
    sp.run(f"make clean -C {coder}", capture_output=True, shell=True)

print("done.")
