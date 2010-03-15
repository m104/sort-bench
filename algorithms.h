#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

/* algorithm definition structure */
typedef struct {
  char *code;               /* brief (command line) ID code */
  char *name;               /* the algorithm name */
  char *description;        /* description info */
  void (*sort)(int *, int); /* the sorting function */
} algorithm;

extern algorithm algorithms[];
extern int algorithm_count;

#endif
