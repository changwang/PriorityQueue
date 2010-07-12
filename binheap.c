#include <stdio.h>
#include <stdlib.h>
#include "binheap.h"

/* function takes a heap rooted at the given index
 * and make sure that is conform to the heap criteria
 */

void heapify(heap_ptr a, int i)
{
    register int l, r, largest;
	
    l = LEFT(i);
    r = RIGHT(i);
	
    /* check the left child */
    largest = ((l <= a->heap_size &&
				compare_priority(a->elements[l], a->elements[i])) ? l : i);
	
    /* check the right child */
    if (r <= a->heap_size &&
		compare_priority(a->elements[r], a->elements[largest]))
    {
        largest = r;
    }
	
    /* swap nodes largest and i, then heapify */
    if (largest != i)
    {
        SWAP(node, a->elements[i], a->elements[largest]);
        heapify(a, largest);
    }
}

/* function to remove the max node from the heap and return it */

node heap_extract_max(heap_ptr a)
{
    node max;
	
    /* if there are elements in the heap, make the last item in the heap
	 the first one, shorten the heap size by 1 and call heapify() */
    if (a->heap_size >= 1)
    {
        max = a->elements[1];
        a->elements[1] = a->elements[(a->heap_size)--];
        heapify(a, 1);
    }
    return max;
}

/* function to insert an element into the heap */

void heap_insert(heap_ptr a, node key)
{
    register int i;
	
    /* if the heap already has the max number of elements,
	 we do not allow more elements to be added */
    if (a->heap_size >= a->max_elems)
    {
        print_error("Heap capacity exceeded, new element not added.");
        return;
    }
	
    /* increase the heap size to accommodate the new node,
	 and set the initial position of this node to be the
	 last node in the heap */
    i = ++(a->heap_size);
	
    /* traverse the path from the leaf to the root to find
	 the proper place for the new node */
    while (i > 1 && compare_priority(key, a->elements[PARENT(i)]))
    {
        a->elements[i] = a->elements[PARENT(i)];
        i = PARENT(i);
    }
	
    /* insert the node at the position that was determined */
    a->elements[i] = key;
}

/* function to delete a node from the heap */

void heap_delete(heap_ptr a, int i)
{
    node deleted;
	
    /* return with an error if the input is invalid */
    if (i > a->heap_size || i < 1)
    {
        sprintf(messages, "heap_delete(): %d, no such element.", i);
        print_error(messages);
        return;
    }
	
    /* switch the element to be deleted with the last item,
	 then shorten the size by 1 */
    deleted = a->elements[i];
    a->elements[i] = a->elements[(a->heap_size)--];
    heapify(a, i);
}

/* function to initialize a given binary heap */

void heap_initialize(heap_ptr a, int nodes)
{
    /* set the size to 0, since a newly created heap has no elements */
    a->heap_size = 0;
	
    /* set max elements to the requested number of nodes,
	 and allocate enough space for this + 1 number of nodes,
	 since heap is always numbered from 1 */
    a->max_elems = nodes;
    a->elements = (node_ptr)malloc(sizeof(node) * ((a->max_elems) + 1));
}

/* function to clean up after we are done with the heap */

void heap_finalize(heap_ptr a)
{
    FREE(a->elements);
}

/* function to print out the elements in the heap,
 * mainly used for debug
 */

void heap_print(binary_heap a)
{
    register int i;
    for (i = 0; i < a.heap_size; i++)
    {
        printf("the element starts from %.15f and ends with %.15f, "
			   "result is: %.15f and error is: %.15f \n",
			   a.elements[i+1].start, a.elements[i+1].end,
			   a.elements[i+1].result, a.elements[i+1].error);
    }
}

/* function to create a node */

node node_create(double start, double end)
{
    node n;
    n.start = start;
    n.end = end;
    n.error = 0.0;
    n.result = 0.0;
    return n;
}

/* function to compare the priority of two given nodes */

int compare_priority(node i, node j)
{
    if (i.error > j.error)
    { return 1; }
    else
    { return 0; }
}

/* function to find if a node is in the heap */

int node_find(binary_heap a, double start)
{
    register int i;
    for (i = 1; i < a.heap_size; i++)
    {
        if (start == a.elements[i].start)
        {
            return i;
        }
    }
    return FAILED;
}

/* function to print out the content of the node,
 * mainly used for debugging.
 */

void node_print(node n)
{
	printf("Node start from %.15f end with %.15f, "
		   "current result is: %.15f, current error is: %.15f\n",
		   n.start, n.end, n.result, n.error);
}

/* function to print an error message */
void print_error(char *msg)
{
    printf("# ERROR: %s\n", msg);
}
