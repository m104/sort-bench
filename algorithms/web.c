#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "web.h"

/*
 * qsort_web1 via:
 *   http://www.daniweb.com/code/snippet216323.html
 *
 * qsort_web2 via:
 *   http://www.alienryderflex.com/quicksort/
 *
 * qsort_web3 via:
 *   http://jeffreystedfast.blogspot.com/2007/03/quick-sort.html
 */

#define CUTOFF 53
#define SWAP stack_swap

struct pivots {
  int left, right;
};

int median ( int list[], int left, int right );
struct pivots partition ( int list[], int left, int right );
void quicksort_r ( int list[], int left, int right );
void quicksort ( int list[], int left, int right );

int median ( int list[], int left, int right )
{
  /* Find the median of three values in list, use it as the pivot */
  int mid = ( left + right ) / 2;

  if ( list[left] > list[mid] )
    SWAP ( &list[left], &list[mid] );

  if ( list[left] > list[right] )
    SWAP ( &list[left], &list[right] );

  if ( list[mid] > list[right] )
    SWAP ( &list[mid], &list[right] );

  SWAP ( &list[mid], &list[right - 1] );

  return list[right - 1];
}

struct pivots partition ( int list[], int left, int right )
{
  int k;
  int i = left, j = right - 1;
  int m = left, n = right - 1;
  int pivot  = median ( list, left, right );
  struct pivots ret;

  /* Three way partition <,==,> */
  for ( ; ; ) {
    while ( list[++i] < pivot )
      ;
    while ( list[--j] > pivot ) {
      if ( j == left )
        break;
    }

    if ( i >= j )
      break;

    SWAP ( &list[i], &list[j] );

    if ( list[i] == pivot )
      SWAP ( &list[++m], &list[i] );

    if ( list[j] == pivot )
      SWAP ( &list[--n], &list[j] );
  }

  SWAP ( &list[i], &list[right - 1] );

  j = i - 1;
  i = i + 1;

  for ( k = left; k < m; k++, j-- )
    SWAP ( &list[k], &list[j] );

  for ( k = right - 1; k > n; k--, i++ )
    SWAP ( &list[k], &list[i] );

  ret.left = i;
  ret.right = j;

  return ret;
}

void quicksort_r ( int list[], int left, int right )
{
  /* Terminate on small subfiles */
  if ( left + CUTOFF <= right ) {
    struct pivots pivot = partition ( list, left, right );

    quicksort_r ( list, left, pivot.right );
    quicksort_r ( list, pivot.left, right );
  }
}

void quicksort1 ( int list[], int left, int right )
{
  quicksort_r ( list, left, right - 1);

  /* Insertion sort on almost sorted list */
  ins_sort( list, list + right - 1);
}



void quicksort2(int *arr, int elements) {
  #define  MAX_LEVELS  1000
  int piv;
  int beg[MAX_LEVELS];
  int end[MAX_LEVELS];
  int i = 0;
  int L;
  int R;

  beg[0] = 0;
  end[0] = elements;

  while (i >= 0) {
    L = beg[i];
    R = end[i] - 1;

    if (L < R) {
      piv = arr[L];
      if (i == MAX_LEVELS-1)
        return;
      while (L < R) {
        while (arr[R] >= piv && L<R)
          R--;
        if (L < R)
          arr[L++] = arr[R];
        while (arr[L] <= piv && L<R)
          L++;
        if (L < R)
          arr[R--] = arr[L];
      }
      arr[L] = piv;
      beg[i + 1] = L + 1;
      end[i + 1] = end[i];
      end[i++] = L;
    } else {
      i--;
    }
  }
  return;
}

typedef struct qstack {
    size_t lo;
    size_t hi;
} qstack_t;

void quicksort3 (int a[], size_t n) {
    qstack_t stack[32], *sp;
    register size_t lo, hi;
    size_t low, high;
    int pivot, tmp;

    if (n < 2)
        return;

    /* push our initial values onto the stack */
    sp = stack;
    sp->lo = 0;
    sp->hi = n;
    sp++;

    while (sp > stack) {
        /* pop lo and hi off the stack */
        sp--;
        high = sp->hi;
        low = sp->lo;

        hi = high - 1;
        lo = low;

        pivot = a[lo];

        while (1) {
            while (lo < high && a[lo] < pivot)
                lo++;

            while (hi > low && a[hi] >= pivot)
                hi--;

            if (lo < hi) {
                /* swap */
                tmp = a[lo];
                a[lo] = a[hi];
                a[hi] = tmp;
                hi--;
            } else {
                hi++;

                if (hi == high) {
                    /* done with this segment */
                    break;
                }

                /* push the larger segment onto the
                 * stack and continue sorting the
                 * smaller segment. */
                if ((hi - low) > (high - hi)) {
                    sp->lo = low;
                    sp->hi = hi;
                    sp++;

                    hi = high;
                    low = lo;
                } else {
                    sp->hi = high;
                    sp->lo = hi;
                    sp++;

                    high = hi;
                    lo = low;
                }

                pivot = a[lo];
                hi--;
            }
        }
    }
}


/* proxy functions, defined in algorithms.h */
void qsort_web1(int *values, int n) {
  quicksort1(values, 0, n);
}
void qsort_web2(int *values, int n) {
  quicksort2(values, n);
}
void qsort_web3(int *values, int n) {
  quicksort3(values, n);
}
