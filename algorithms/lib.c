#include "lib.h"


/* standard integer comparison function */
int mycompare(const void *px, const void *py) {
  int x = *((int *) px);
  int y = *((int *) py);

  if (x > y) return 1;
  if (x < y) return -1;
  return 0;
}

/* proxy functions, defined in algorithms.h */
void lib_qsort(int *values, int n) {
  qsort(values, n, sizeof(values[0]), mycompare);
}
#ifdef __DARWIN_UNIX03
void lib_msort(int *values, int n) {
  mergesort(values, n, sizeof(values[0]), mycompare);
}
void lib_hsort(int *values, int n) {
  heapsort(values, n, sizeof(values[0]), mycompare);
}
#endif
