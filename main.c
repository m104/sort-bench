#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "algorithms.h"
#include "algorithms/lib.h"


char *VERSION = "0.1.0";

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
  printf("usage: sort-bench <OPTIONS> [NUMBER [NUMBER ...]] \n");
  printf("\n");
  printf("options:\n");
  printf("    --help, -h:  usage information\n");
  printf("            -v:  verbose mode\n");
  printf("            -l:  list algorithms\n");
  printf("  -a ALGORITHM:  use a specific algorithm (default: lib_qsort)\n");
  printf("     -t TRIALS:  perform a number of trials (default: 1)\n");
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
    printf("%i. %s - %s\n", i + 1, algo->code, algo->name);
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
  /* trial outcome */
  int outcome;
  int diffs;
  /* argument processing */
  char option = ' ';
  char *arg;
  int arg_offset;
  /* temporary */
  int i;
  int j;
  char *rest;

  /* defaults */
  algo = algorithms;
  option = ' ';
  verbose = 0;
  trials = 1;
  arg_offset = 1;

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
        } else {
          /* stop processing args */
          i = argc;
          arg_offset--;
        }
    }
  }

  n = argc - arg_offset;
  originals = astrtoal((const char **) argv + arg_offset, n);

  bytes = sizeof(int) * n;
  values = malloc(bytes);
  sorted = malloc(bytes);

  /* pre-sort an array, just for later comparison */
  memcpy(sorted, originals, bytes);
  lib_qsort(sorted, n);

  /* algorithm selection */
  sort = algo->sort;

  printf("Algorithm: %s (%s)\n", algo->code, algo->name);
  printf("Trials: %i\n", trials);
  printf("Values: %i\n", n);

  if (verbose) {
    printf("Unsorted Array:\n");
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
      printf("verdict: FAIL\n");
      printf(" incorrect values: %i of %i\n", diffs, n);
      outcome = -1;
    } else {
      printf("Verdict: pass\n");
    }
  }

  free(sorted);
  free(values);
  free(originals);

  return outcome;
}

