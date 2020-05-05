#include "io.h"
#include "lzw.h"
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
    "   Decompresses files with the LZW decompression algorithm.\n"
    "   Compressed files are compressed with the corresponding encoder.\n"
    "\n"
    "USAGE\n"
    "   %s -[hv] [-i input] [-o output]\n"
    "\n"
    "OPTIONS\n"
    "   -h          Display program usage\n"
    "   -v          Display decompression statistics\n"
    "   -i input    Specify input to decompress (stdin by default)\n"
    "   -o output   Specify output to decompress to (stdout by default)\n",
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
      outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC);
      check(outfile != -1, "Failed to open %s.\n", optarg);
      break;
    default:
      print_usage(argv);
      return 1;
    }
  }

  FileHeader header = { 0, 0 };
  read_header(infile, &header);

  check(header.magic == MAGIC, "Bad magic number.\n");
  fchmod(outfile, header.protection);

  decode(infile, outfile);

  if (verbose) {
    double ratio = 1.0 - (double) bytes(total_bits) / (double) total_syms;
    fprintf(stderr, "Compressed size: %" PRIu64 " bytes\n", bytes(total_bits));
    fprintf(stderr, "Uncompressed size: %" PRIu64 " bytes\n", total_syms);
    fprintf(stderr, "Compression ratio: %.2lf%%\n", 100.0 * ratio);
  }

  close(infile);
  close(outfile);

  return 0;
}
