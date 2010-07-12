#ifndef UTILS_H_
#define UTILS_H_

#include "binheap.h"

#define ABSREQ		1e-12					/* required absolute accuracy */
#define ITMAX		10000000				/* max number iterations */

#define min(a, b)	((a) <= (b) ? (a) : (b))	/* get minimum element from two */
#define max(a, b)	((a) >= (b) ? (a) : (b))	/* get maximum element from two */

/* mode of update on global states */

typedef enum mode { subtraction = 0, addition } mode;

/* function that will be used in test */

void rule(double (* f)(double), node_ptr);

#endif /* UTILS_H_ */
