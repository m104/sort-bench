#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "algorithms.h"
#include "algorithms/lib.h"


char *VERSION = "0.2.0";

void print_version() {
  printf("Sort Bench, version %s\n", VERSION);
  printf("\n");
}

void print_legal() {
  printf("Copyright (C) 2010 Adam Collins (m104)\n");
  printf("This is free software; see the source (http://github.com/m104/sort-bench)\n");
  printf("for copying conditions.  There is NO warranty; not even for MERCHANTABILITY\n");
  printf("or FITNESS FOR A PARTICULAR PURPOSE.\n");
  printf("\n");
}

void print_usage() {
  printf("usage: sort-bench [<OPTIONS>] FILE \n");
  printf("\n");
  printf("options:\n");
  printf("     --version:  version and legal information\n");
  printf("    --help, -h:  usage information\n");
  printf("            -v:  verbose\n");
  printf("            -q:  quiet\n");
  printf("            -l:  list algorithms\n");
  printf("  -a ALGORITHM:  use a specific algorithm (default: lib_qsort)\n");
  printf("     -t TRIALS:  perform a number of trials (default: 1)\n");
  printf("\n");
  printf("FILE should contain a number of integers, one per line\n");
  printf("\n");
}

void print_algorithms_list() {
  int i;
  algorithm *algo;

  printf("\n");
  printf("Algorithm List:\n");
  printf("\n");

  for (i = 0; i < algorithm_count; i++) {
    algo = algorithms + i;
    printf("[ %s ]  %s\n", algo->code, algo->name);
    printf("%s\n", algo->description);
    printf("\n");
  }
}

int main(int argc, char **argv) {
  /* array storage */
  int *originals;
  int *values;
  int *sorted;
  int n;
  int bytes;
  /* configurable */
  algorithm *algo;
  void (*sort)(int *, int);
  int trials;
  int verbose;
  int quiet;
  /* trial outcome */
  int outcome;
  int diffs;
  /* argument processing */
  char option = ' ';
  char *arg;
  int arg_offset;
  /* file processing */
  FILE *file;
  const char *filename;
  char buffer[1024];
  /* temporary */
  int i;
  int j;
  char *rest;

  /* defaults */
  algo = algorithms;
  option = ' ';
  verbose = 0;
  quiet = 0;
  trials = 1;
  arg_offset = 1;
  file = stdin;
  filename = "STDIN";

  /* parse the arguments */
  for (i = 1; i < argc; i++) {
    arg = argv[i];
    arg_offset++;

    switch (option) {
      case 'a':
        for (j = 0; j < algorithm_count; j++) {
          if (strcmp(arg, algorithms[j].code) == 0) {
            algo = algorithms + j;
            break;
          }
        }
        option = ' ';
        break;

      case 't':
        trials = strtol(arg, &rest, 10);
        if (trials < 1) trials = 1;
        option = ' ';
        break;

      default:
        if (strcmp(arg, "--version") == 0) {
          print_version();
          print_legal();
          return 0;
        } else if (strcmp(arg, "--help") == 0) {
          print_version();
          print_usage();
          return 0;
        } else if (strcmp(arg, "-h") == 0) {
          print_version();
          print_usage();
          return 0;
        } else if (strcmp(arg, "-l") == 0) {
          print_algorithms_list();
          return 0;
        } else if (strcmp(arg, "-a") == 0) {
          option = 'a';
        } else if (strcmp(arg, "-t") == 0) {
          option = 't';
        } else if (strcmp(arg, "-v") == 0) {
          verbose = 1;
          quiet = 0;
        } else if (strcmp(arg, "-q") == 0) {
          quiet = 1;
          verbose = 0;
        } else if (strcmp(arg, "--") == 0) {
          i = argc;
        } else { /* not recognized */
          i = argc;
        }
    }
  }

  n = argc - --arg_offset;

  /* any arguments left? */
  if (n > 0) {
    /* file or stdin? */
    arg = argv[arg_offset];
    if (strcmp(arg, "-") != 0) {
      filename = arg;
      if (!(file = fopen(filename, "r"))) {
        perror("Could not open file");
        return -1;
      }
    }
  }

  /* start with a small number of values */
  n = 1024;
  bytes = sizeof(int) * n;
  originals = malloc(bytes);

  for (i = 0; fgets(buffer, sizeof(buffer), file); i++) {
    if (i + 1 > n) {
      /* allocate 25% more space for our numbers */
      n += n / 4;
      bytes = sizeof(int) * n;
      originals = realloc(originals, bytes);
    }
    j = strtol(buffer, &rest, 10);
    originals[i] = j;
  }

  n = i;

  bytes = sizeof(int) * n;
  values = malloc(bytes);
  sorted = malloc(bytes);

  /* pre-sort an array, just for later comparison */
  memcpy(sorted, originals, bytes);
  lib_qsort(sorted, n);

  /* algorithm selection */
  sort = algo->sort;

  if (!quiet) {
    printf("Algorithm: %s (%s)\n", algo->code, algo->name);
    printf("Trials: %i\n", trials);
    printf("Source: %s\n", filename);
    printf("Values: %i\n", n);
  }

  if (verbose) {
    printf("\nUnsorted Array:\n");
    print_array(originals, n);
    printf("\n");
  }

  /* run the benchmark */
  while (trials--) {
    memcpy(values, originals, bytes);
    sort(values, n);
  }

  if (verbose) {
    printf("Final Array:\n");
    print_array(values, n);
    printf("\n");
  }

  /* validate this run */
  outcome = 0;

  if (n > 0) {
    diffs = array_differences(values, sorted, n);
    if (diffs) {
      if (!quiet) {
        printf("verdict: FAIL\n");
        printf(" incorrect values: %i of %i\n", diffs, n);
      }
      outcome = -1;
    } else {
      if (!quiet) {
        printf("Verdict: pass\n");
      }
    }
  }

  fclose(file);
  free(sorted);
  free(values);
  free(originals);

  return outcome;
}

