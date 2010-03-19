#ifndef __LIB_H__
#define __LIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"

/* GNU library sorting */
void lib_qsort(int *values, int n);
#ifdef __DARWIN_UNIX03
void lib_msort(int *values, int n);
void lib_hsort(int *values, int n);
#endif /* __DARWIN_UNIX03 */

#endif

