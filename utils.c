#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP stack_swap

/* print out the ints in an array */
void print_array(const int *start, int n) {
  const int *left;
  const int *end;

  end = start + n - 1;
  left = start;

  printf("  %i: [", n);
  while (left <= end)
    printf(" %i", *(left++));
  printf(" ]\n");
}

void inline stack_swap(int *x, int *y) {
  int t;
  t = *x;
  *x = *y;
  *y = t;
}

/* slower than stack_swap, but I had to try */
void inline inplace_swap(int *x, int *y) {
  *x ^= *y;
  *y ^= *x;
  *x ^= *y;
}

/* fastest way to sort two values */
void inline sort2(int *x, int *y) {
  if (*x > *y)
    SWAP(x, y);
}

/* fastest way to sort three values? */
void inline sort3(int *x, int *y, int *z) {
  sort2(x, z);
  sort2(x, y);
  sort2(y, z);
}

/* generic int insertion sort
 * my testing indicates that this should be used for
 * arrays in the size range of 4 - 52 */
void ins_sort(int *start, int *end) {
  int *left;
  int *right;
  int n;
  int value;

  n = end - start + 1;

  if (n < 1) return;

  if (n == 2) {
    sort2(start, end);
    return;
  }

  if (n == 3) {
    sort3(start, start + 1, end);
    return;
  }

  left = start;

  for (left = start; left <= end; left++) {
    value = *left;

    for (right = left - 1; right >= start; right--) {
      if (*right <= value)
        break;
      *(right + 1) = *right;
    }
    *(right + 1) = value;
  }
}

/* check for monotonic bias
 * gives a value that can be compared with the length
 * of the array to determine how "sorted" it is */
int inline monotonic_bias(int *start, int *end) {
  int *left;
  int bias;
  int prev;

  prev = *start;
  bias = 0;

  for (left = start + 1; left <= end; left++) {
    if (prev <= *left)
      bias++;
    else
      bias--;
    prev =*left;
  }

  return bias;
}

/* reverse the array */
void inline reverse_array(int *start, int *end) {
  int *left;
  int *right;

  left = start;
  right = end;

  while (left < right)
    SWAP(left++, right--);
}

/* convert an array of strings to an array of ints,
 * like from, say, the command line arguments */
int * astrtoal(const char **strings, int n) {
  int bytes;
  int *values;
  int i;
  char *rest;

  bytes = sizeof(int) * n;
  values = malloc(bytes);

  for (i = 0; i < n; i++) {
    values[i] = strtol(strings[i], &rest, 10);
  }

  return values;
}

/* counts how many differences there are between
 * two (equally sized) arrays */
int array_differences(const int *values1, const int *values2, int n) {
  const int *l1;
  const int *l2;
  const int *end;
  int diffs;

  end = values1 + n - 1;
  diffs = 0;

  for (l1 = values1, l2 = values2; l1 <= end; l1++, l2++) {
    if (*l1 != *l2)
      diffs++;
  }

  return diffs;
}

