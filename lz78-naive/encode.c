#include "io.h"
#include "lz78.h"
#include "util.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define OPTIONS "hvi:o:"

void print_usage(char **argv) {
  fprintf(stderr,
    "SYNOPSIS\n"
    "   Compresses files using the LZ78 compression algorithm.\n"
    "   Compressed files are decompressed with the corresponding decoder.\n"
    "\n"
    "USAGE\n"
    "   %s [-hv] [-i input] [-o output]\n"
    "\n"
    "OPTIONS\n"
    "   -h          Display program help and usage\n"
    "   -v          Display compression statistics\n"
    "   -i input    Specify input to compress (stdin by default)\n"
    "   -o output   Specify output of compressed input (stdout by default)\n",
    argv[0]);
  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  int infile = STDIN_FILENO;
  int outfile = STDOUT_FILENO;
  bool verbose = false;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      print_usage(argv);
      return 0;
    case 'v':
      verbose = true;
      break;
    case 'i':
      infile = open(optarg, O_RDONLY);
      check(infile != -1, "Failed to open %s.\n", optarg);
      break;
    case 'o':
      outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      check(outfile != -1, "Failed to open %s.\n", optarg);
      break;
    default:
      print_usage(argv);
      return 1;
    }
  }

  struct stat statbuf;
  fstat(infile, &statbuf);
  fchmod(outfile, statbuf.st_mode);

  FileHeader header = {
    .magic = MAGIC,
    .protection = statbuf.st_mode
  };

  write_header(outfile, &header);

  encode(infile, outfile);

  if (verbose) {
    double ratio = 1.0 - (double) bytes(total_bits) / (double) total_syms;
    printf("Compressed size: %" PRIu64 " bytes\n", bytes(total_bits));
    printf("Uncompressed size: %" PRIu64 " bytes\n", total_syms);
    printf("Compression ratio: %.2lf%%\n", 100.0 * ratio);
  }

  close(infile);
  close(outfile);

  return 0;
}
