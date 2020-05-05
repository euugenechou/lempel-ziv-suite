#include "io.h"
#include "trie.h"
#include "util.h"
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
    "   Compresses files using the LZW compression algorithm.\n"
    "   Compressed files are decompressed with the corresponding decoder.\n"
    "\n"
    "USAGE\n"
    "   %s -[vh] -i <input file> -o <output file>\n"
    "\n"
    "OPTIONS\n"
    "   -v: Display compression statistics\n"
    "   -i: Set input as argument after '-i' (stdin by default)\n"
    "   -o: Set output as argument after '-o' (stdout by default)\n"
    "   -h: Display program usage\n",
    argv[0]);
  return;
}

void encode(int infile, int outfile) {
  struct stat statbuf;
  fstat(infile, &statbuf);
  fchmod(outfile, statbuf.st_mode);

  FileHeader header = { MAGIC, statbuf.st_mode };
  write_header(outfile, &header);

  TrieNode *root = trie_create();
  TrieNode *curr_node = root;
  uint8_t curr_sym = 0;
  uint16_t next_code = START_CODE;

  while (read_sym(infile, &curr_sym)) {
    TrieNode *next_node = trie_step(curr_node, curr_sym);

    if (next_node) {
      curr_node = next_node;
    } else {
      buffer_code(outfile, curr_node->code, bitwidth(next_code));
      curr_node->children[curr_sym] = trie_node_create(next_code++);
      curr_node = root->children[curr_sym];
    }

    if (next_code == MAX_CODE) {
      trie_reset(root);
      curr_node = root->children[curr_sym];
      next_code = START_CODE;
    }
  }

  if (curr_node != root) {
    buffer_code(outfile, curr_node->code, bitwidth(next_code++));

    if (next_code == MAX_CODE) {
      next_code = START_CODE;
    }
  }

  buffer_code(outfile, STOP_CODE, bitwidth(next_code));
  flush_codes(outfile);
  trie_delete(root);
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

  encode(infile, outfile);

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
