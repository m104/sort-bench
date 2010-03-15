#ifndef __UTILS_H__
#define __UTILS_H__

/* print out the ints in an array */
void print_array(const int *start, int n);

void inline stack_swap(int *x, int *y);

/* slower than stack_swap, but I had to try */
void inline inplace_swap(int *x, int *y);

/* fastest way to sort two values */
void inline sort2(int *x, int *y);

/* fastest way to sort three values? */
void inline sort3(int *x, int *y, int *z);

/* generic int insertion sort
 * my testing indicates that this should be used for
 * arrays in the size range of 4 - 52 */
void ins_sort(int *start, int *end);

/* convert an array of strings to an array of ints,
 * like from, say, the command line arguments */
int * astrtoal(const char **strings, int n);

/* check for monotonic bias
 * gives a value that can be compared with the length
 * of the array to determine how "sorted" it is */
int inline monotonic_bias(int *start, int *end);

/* reverse an array */
void inline reverse_array(int *start, int *end);

/* counts how many differences there are between
 * two (equally sized) arrays */
int array_differences(const int *values1, const int *values2, int n);

#endif
