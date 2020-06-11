#!/usr/local/bin/python3

import os
import sys
import time
import argparse
import subprocess as sp

parser = argparse.ArgumentParser()
parser.add_argument("-c", help="compress", action="store_true")
parser.add_argument("-d", help="decompress", action="store_true")
parser.add_argument("-i", help="input", nargs="?")
args = parser.parse_args()

coder = "lz78-naive"
e = coder + "/encode"
d = coder + "/decode"

sp.run(f"make -C {coder}", capture_output=True, shell=True)

end = 0
start = 0
duration = 0
filesize = 0
throughput = 0

if (args.c):
    infile = args.i
    outfile = f"/tmp/{os.path.basename(infile)}.zipped"

    start = time.time()
    sp.run(f"{e} -i {infile} -o {outfile}", capture_output=True, shell=True)
    end = time.time()

    filesize = os.stat(outfile).st_size

if (args.d):
    infile = f"/tmp/{os.path.basename(args.i)}.zipped"
    outfile = f"/tmp/{os.path.basename(args.i)}"

    start = time.time()
    sp.run(f"{d} -i {infile} -o {outfile}", capture_output=True, shell=True)
    end = time.time()

    filesize = os.stat(outfile).st_size

duration = end - start
throughput = (filesize / duration) / 1000000

sp.run(f"make clean -C {coder}", capture_output=True, shell=True)

print(f"{duration} {throughput}")
