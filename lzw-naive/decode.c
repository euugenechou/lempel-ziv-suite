#include "io.h"
#include "util.h"
#include "word.h"
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "vi:o:h"

void print_usage(char **argv) {
  fprintf(stderr,
    "SYNOPSIS\n"
    "   Decompresses files with the LZW decompression algorithm.\n"
    "   Used with files compressed with the corresponding encoder.\n"
    "\n"
    "USAGE\n"
    "   %s -[vh] -i <input file> -o <output file>\n"
    "\n"
    "OPTIONS\n"
    "   -v: Display decompression statistics\n"
    "   -i: Set input as argument after '-i' (stdin by default)\n"
    "   -o: Set output as argument after '-o' (stdout by default)\n"
    "   -h: Display program usage\n",
    argv[0]);
  return;
}

void decode(int infile, int outfile) {
  FileHeader header = { 0, 0 };
  read_header(infile, &header);

  check(header.magic == MAGIC, "Bad magic number.\n");

  fchmod(outfile, header.protection);

  WordTable *wt = wt_create();
  uint16_t curr_code = 0;
  uint16_t prev_code = STOP_CODE;
  uint16_t next_code = START_CODE;

  while (read_code(infile, &curr_code, bitwidth(next_code + 1))) {
    Word *curr_word = wt[curr_code];

    if (prev_code != STOP_CODE) {
      Word *prev_word = wt[prev_code];

      if (curr_word) {
        wt[next_code++] = word_append_sym(prev_word, curr_word->syms[0]);
      } else {
        curr_word = word_append_sym(prev_word, prev_word->syms[0]);
        wt[next_code++] = curr_word;
      }
    }

    buffer_word(outfile, curr_word);
    prev_code = curr_code;

    if (next_code + 1 == MAX_CODE) {
      wt_reset(wt);
      prev_code = STOP_CODE;
      next_code = START_CODE;
    }
  }

  flush_words(outfile);
  wt_delete(wt);
  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  int infile = STDIN_FILENO;
  int outfile = STDOUT_FILENO;
  bool verbose = false;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
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
    case 'h':
      print_usage(argv);
      exit(EXIT_SUCCESS);
    default:
      print_usage(argv);
      exit(EXIT_FAILURE);
    }
  }

  decode(infile, outfile);

  if (verbose) {
    double ratio = 1.0 - (double) bytes(total_bits) / (double) total_syms;
    printf("Compressed size: %" PRIu64 " bytes\n", bytes(total_bits));
    printf("Uncompressed size: %" PRIu64 " bytes\n", total_syms);
    printf("Compression ratio: %.2lf%%\n", 100.0 * ratio);
  }

  close(infile);
  close(outfile);

  return EXIT_SUCCESS;
}
