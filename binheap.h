#ifndef BINHEAP_H_
#define BINHEAP_H_

#define MSGSIZE     128     /* max size of a debug message */
#define FAILED      -1      /* define -1 to indicate failure of some operations */
#define EMPTY       1       /* define 1 to indicate the heap is empty */

#define FREE(x)			free(x); x = NULL		/* core if free'ed pointer is used */
#define LEFT(x)			(2 * x)					/* left child of a node */
#define RIGHT(x)		((2 * x) + 1)			/* right child of a node */
#define PARENT(x)		(x/2)					/* parent of a node */
#define SWAP(t, x, y)	tmp = x; x = y; y = tmp	/* swap to variables */

/* global character array for debug & error message */

char messages[MSGSIZE];

/* define a structure representing an individual node in the heap,
 * and make it a valid type for convenience.
 * meanwhile, use error as the priority.
 */

typedef struct node
{
    double start;
    double end;
    double error;		/* use error as priority, largest in the root */
    double result;
} node;

/* for convenience in function declarations,
 * typedef a pointer to a node as its own type, node_ptr
 */

typedef node *node_ptr;

/* global node tmp for swapping purpose */

node tmp;

/* define a structure representing a heap,
 * and make it a valid type for convenience
 */

typedef struct binary_heap
{
    int heap_size;
    int max_elems;
    node_ptr elements;
} binary_heap;

/* for convenience in function declarations,
 * typedef a pointer to a binary_heap as its own type, heap_ptr
 */

typedef binary_heap *heap_ptr;

/* function prototypes for functions which operate on a binary heap */

void        heapify(heap_ptr a, int i);
node        heap_extract_max(heap_ptr a);
void        heap_insert(heap_ptr a, node key);
void        heap_delete(heap_ptr a, int i);
void        heap_initialize(heap_ptr a, int nodes);
void        heap_finalize(heap_ptr a);
void        heap_print(binary_heap a);

/* function prototypes for functions which operate on a node */

int         node_find(binary_heap a, double start);
node        node_create(double start, double end);
void        node_print(node n);

/* function prototypes for helper functions */

int         compare_priority(node i, node j);
void        print_error(char *msg);

#endif /* BINHEAP_H_ */
