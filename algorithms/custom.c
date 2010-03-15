#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "custom.h"

/* use the standard stack-based swap */
#define SWAP stack_swap

/* global "stack" space */
int *starts[250];
int *ends[250];


/* index-based heap quicksort, simple algorithm */
void heap(int *values, int n) {
  int i;
  int high_i;
  int low_i;
  int part;
  int value;
  int *dup;

  switch (n) {
    case 0:
    case 1:
      return;

    case 2:
      sort2(&values[0], &values[1]);
      return;

    case 3:
      sort3(&values[0], &values[1], &values[2]);
      return;
  }

  /* make a copy */
  dup = malloc(n * sizeof(int));
  memcpy(dup, values, n * sizeof(int));

  /* set the boundaries */
  low_i = 0;
  high_i = n - 1;
  part = values[0];

  /* partition the sucker */
  for (i = 1; i < n; i++) {
    value = values[i];
    if (value < part) {
      dup[low_i] = value;
      low_i++;
    } else {
      dup[high_i] = value;
      high_i--;
    }
  }

  /* place the partition value */
  dup[low_i] = part;

  /* copy back */
  memcpy(values, dup, n * sizeof(int));
  free(dup);

  /* recurse! */
  if (low_i > 1)
    heap(values, low_i); /* lows */
  if (high_i < ( n - 2))
    heap(values + low_i + 1, n - low_i - 1); /* highs */
}


/* index-based recursive in-place quicksort, naive algorithm */
void inplace1(int *values, int n) {
  int i;
  int part_i;
  int part;
  int value;

  if (n < 2)
      return;

  /* set the initial partition */
  part_i = n - 1;
  part = values[part_i];

  /* partition the sucker */
  for (i = 0; i < n - 1; i++) {
    /* no need to compare the partition element */
    if (i == part_i)
      continue;

    value = values[i];

    if (i < part_i) {
      if (value > part) {
        SWAP(&values[i], &values[part_i]);
        part_i = i;
        part = values[part_i];
      }
    } else {
      if (value < part) {
        if (i != part_i + 1)
          SWAP(&values[i], &values[part_i + 1]);
        SWAP(&values[part_i + 1], &values[part_i]);
        part_i++;
        part = values[part_i];
      }
    }
  }

  /* recurse! */
  if (part_i > 1)
    inplace1(values, part_i); /* lows */
  if ((n - part_i) > 2)
    inplace1(values + part_i, n - part_i); /* highs */
}


/* index-based recurvsive in-place quicksort, simplified algorithm */
void inplace2(int *values, int start, int end) {
  int left;
  int right;
  int n;
  int pivot;

  n = end - start + 1;

  if (n < 2)
    return;

  /* can we cut out early? */
  switch (n) {
    case 2:
      sort2(&values[start], &values[end]);
      return;

    case 3:
      sort3(&values[start], &values[start+1], &values[end]);
      return;
  }

  pivot = values[end];
  left = start;
  right = end - 1;

  while (left < right) {
    if (values[left] <= pivot) {
      left++;
    } else {
      SWAP(&values[left], &values[right--]);
    }
  }
  sort2(&values[right], &values[end]);

  /* recurses! */
  inplace2(values, start, left);
  inplace2(values, right + 1, end);
}


/* pointer-based stack-iterative in-place quicksort, optimized algorithm 
 * (not as fast as you may think!) */
void inplace3(int *first, int *last) {
  /* stack storage */
  int *starts[250];
  int *ends[250];
  int **current_start;
  int **current_end;
  int **home_start;

  /* iteration storage */
  int *start;
  int *end;
  int *left;
  int *right;
  int n;
  int pivot;

  home_start = starts;
  current_start = starts;
  current_end = ends;

  /* start off with the whole array */
  *current_start = first;
  *current_end = last;

  while (current_start >= home_start) {
    start = *current_start;
    end = *current_end;
    n = end - start + 1;

    /* special cases! */
    if (n < 53) {
      /* we're done with this iteration */
      current_start--;
      current_end--;

      /* handle this part of the array */
      if (n > 3)
        ins_sort(start, end);
      else if (n == 2)
        sort2(start, end);
      else if (n == 3)
        sort3(start, start + 1, end);

      continue;
    }

    pivot = *end;
    left = start;
    right = end - 1;

    /* partition the space */
    while (left < right) {
      if (*left <= pivot) {
        left++;
      } else {
        SWAP(left, right--);
      }
    }

    /* minor correction because *right may not have been tested */
    if (*right < pivot)
      right++;

    /* don't perform a useless swap */
    if (right != end)
      /* place the pivot point */
      SWAP(right, end);

    /* recurses! */
    *current_start = start;
    *current_end = right - 1;
    *(++current_start) = right + 1;
    *(++current_end) = end;
  }
}


/* pointer-based recursive in-place quicksort, optimized algorithm */
void hybrid(int *start, int *end) {
  int *left;
  int *mid;
  int *right;
  int *inner;
  int *outer;
  int n;
  int pivot;

  n = end - start + 1;

  if (n < 53)
    return;

  /* median selection pivot point */
  mid = start + (n / 2);
  sort3(start, mid, end);

  SWAP(mid, end - 1);
  pivot = *(end - 1);
  inner = left = start;
  outer = right = end - 1;

  /* subtle, but we're not going to partition the whole
   * array, just (left + 1) to (right - 1) */
  while (1) {
    while (*(++left) < pivot);

    while (*(--right) > pivot)
      if (right == start)
        break;

    if (left >= right)
      break;

    SWAP(left, right);

    if (*left == pivot)
      SWAP(++inner, left);

    if (*right == pivot)
      SWAP(--outer, right);
  }

  SWAP(left, end - 1);

  right = left++ - 1;

  for (mid = start; mid < inner;)
    SWAP(mid++, right--);

  for (mid = end - 1; mid > outer;)
    SWAP(mid--, left++);

  /* is the whole array identical? */
  if (*start == *end)
    return;

  /* recurses! */
  hybrid(start, right);
  hybrid(left, end);
}


/* proxy functions, defined in algorithms.h */
void qsort_heap(int *values, int n) {
  heap(values, n);
}
void qsort_inplace1(int *values, int n) {
  inplace1(values, n);
}
void qsort_inplace2(int *values, int n) {
  inplace2(values, 0, n - 1);
}
void qsort_inplace3(int *values, int n) {
  inplace3(values, values + n - 1);
}
void qsort_hybrid(int *start, int n) {
  int *end;

  end = start + n - 1;

  hybrid(start, end - 1);
  ins_sort(start, end);
}

