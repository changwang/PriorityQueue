#ifdef _OPENMP
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "utils.h"

double TOTALRESULT; 	/* global result */
double TOTALERROR; 	/* global error */

/* indicates how the computation terminate
 * 0 means estimated error is less than
 * given required absolute accuracy.
 * 1 means number of iterations reaches
 * the given iteration number.
 */
int RETURNFLAG;

double smooth(double);
double oscillating(double);
double peaked(double);
double singular(double);

void update(node, mode);

int main(int argc, char const *argv[])
{
	//	int tid;		/* thread id */
	struct timeval tt;
	float e;
	long tsec, tusec;
	
	node max_local, sub_local1, sub_local2;
	
	/* initialize global state */
	TOTALRESULT = 0.0;
	TOTALERROR = 0.0;
	
	int iter = 1;
	
	/* retrieve the start computing time */
	gettimeofday(&tt, 0);
	tsec = tt.tv_sec;
	tusec = tt.tv_usec;

	/* create the binary heap */
	binary_heap h;
	heap_initialize(&h, 2*ITMAX);
	/* give the binary heap a root node */
	node root = node_create(0, 1);
	rule(smooth, &root);
	update(root, addition);
	heap_insert(&h, root);
	//    heap_print(h);
	
#ifdef _OPENMP
	/* set the number of threads */
	omp_set_num_threads(8);

#pragma omp parallel shared(h, iter, TOTALRESULT, TOTALERROR) private(max_local, sub_local1, sub_local2)
#endif
{
	while ((TOTALERROR > ABSREQ) && (iter < ITMAX))
	{
#ifdef _OPENMP
#pragma omp critical (retrive_max)
#endif
			{
				/* this section should be a critical section,
				 * because each time only one thread can access
				 * the binary heap */
#ifdef _OPENMP
//				printf("Current thread %d is handling the heap, and iteration is %d.\n",
//					   omp_get_thread_num(), iter);
#endif
				max_local = heap_extract_max(&h);
				//				if (max_local.result == 0.0) continue;
				//				node_print(max_local);
			}
			/* divide the max error region into two small regions */
			sub_local1 = node_create(max_local.start, (max_local.start
													   + max_local.end) / 2.0);
			sub_local2 = node_create((max_local.start + max_local.end) / 2.0,
									 max_local.end);
#ifdef _OPENMP
#pragma omp critical (state_update)
#endif
			{
				/* critical section, only one thread can access
				 * the global state each time */
				update(max_local, subtraction);
			}
			/* apply the function, update the result and error */
			rule(smooth, &sub_local1);
			rule(smooth, &sub_local2);
#ifdef _OPENMP
#pragma omp critical (heap_update)
#endif
			{
				/* critical section, only one thread can access
				 * the global state, binary heap
				 * and variable iter each time */
				update(sub_local1, addition);
				update(sub_local2, addition);
				heap_insert(&h, sub_local1);
				heap_insert(&h, sub_local2);
				iter++;
			}
		}
}
	//    heap_print(h);
	
	/* get the current time, representing the end computing time */
	gettimeofday(&tt, 0);
	e = (float) (tt.tv_sec - tsec) + (float) (tt.tv_usec - tusec) / 1000000;
	printf("It took %g seconds to get the final result %.15f\n", e, TOTALRESULT);
	
	heap_finalize(&h);
	printf("total error is: %.40f, and number of iteration is: %d\n", TOTALERROR, iter);
	return 0;
}

/* update the global state */

void update (node n, mode m)
{
	/* if mode equals to 0, which means to subtract,
	 * if mode equals to 1, which means to add */
	if (m == addition)
	{
		TOTALRESULT += n.result;
		TOTALERROR = fabs(TOTALERROR + n.error);
	}
	else if (m == subtraction)
	{
		TOTALRESULT -= n.result;
		TOTALERROR = fabs(TOTALERROR - n.error);
	}
	else
	{
		return;
	}
}

/* simple test function, here I want to estimate PI */

/* approximate pi */

double smooth(double t)
{
	return pow(t, 9.0);
}

/* approximate e - 1 */

double oscillating(double t)
{
	return cos(1000 * t);
}

/* approximate 2 */

double peaked(double t)
{
	return 1.0/(t * t + pow(10, -8));
}

/* approximate sin(x)^2 / 2 */

double singular(double t)
{
	if (t == 0)
	{
		return 0.0;
	}
	else
	{
		return 1.0/sqrt(t);
	}
}

