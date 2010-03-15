#include "algorithms.h"
#include "algorithms/lib.h"
#include "algorithms/web.h"
#include "algorithms/custom.h"

int algorithm_count = 11;

algorithm algorithms[] = {
  { "qsort",
    "Standard Quicksort",
    "System-dependant qsort()",
    &lib_qsort },

  { "msort",
    "Standard Merge Sort",
    "System-dependant mergesort()",
    &lib_msort },

  { "hsort",
    "Standard Heap Sort",
    "System-dependant heapsort()",
    &lib_hsort },

  { "qsort_hybrid",
    "QuickSort - optimized hybrid",
    "In-place pointer-based optimized quicksort, hybridized with insertion sort",
    &qsort_hybrid },

  { "qsort_web1",
    "QuickSort - web optimized #1",
    "In-place recursive hybrid quicksort, via: http://www.daniweb.com/code/snippet216323.html",
    &qsort_web1 },

  { "qsort_web2",
    "QuickSort - web optimized #2",
    "In-place stack-optimized quicksort, via: http://www.alienryderflex.com/quicksort/",
    &qsort_web2 },

  { "qsort_web3",
    "QuickSort - web optimized #3",
    "In-place stack-optimized quicksort, via: http://jeffreystedfast.blogspot.com/2007/03/quick-sort.html",
    &qsort_web3 },

  { "qsort_inplace3",
    "QuickSort - in-place #3",
    "In-place pointer-based stack-pointer optimized quicksort",
    &qsort_inplace3 },

  { "qsort_inplace2",
    "QuickSort - in-place #2",
    "In-place index-based recursive quicksort, simplified, but not optimized",
    &qsort_inplace2 },

  { "qsort_inplace1",
    "QuickSort - in-place #1",
    "On-place index-based recursive quicksort, not optimized",
    &qsort_inplace1 },

  { "qsort_heap",
    "QuickSort - heap",
    "Heap index-based quicksort, not optimized",
    &qsort_heap }
};

