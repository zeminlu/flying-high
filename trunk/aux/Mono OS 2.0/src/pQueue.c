/*
 * pQueue.c
 *
 *  Created on: Oct 25, 2008
 *      Author: Santiago Perez De Rosso
 */

/*
 * System includes
 * ===============
 *
 */
#include "stdio.h"
#include "stdlib.h"

/*
 * Project includes
 * ================
 *
 */
#include "debug.h"
#include "pQueue.h"
#include "alloc.h"

/*
 * Defines
 * =======
 *
 */

/**
 * 	\struct		queueElemT
 * 	\brief		holds the information of a queue element. \a priority is the priority the element has within the queue and
 * 				\a data is the associated that to that queue element
 */
typedef struct queueElemT
{
	float priority;
	void *data;
} queueElemT;

/**
 * 	\struct		pQueueCDT
 * 	\brief		holds the header of the priority queue. \a heap is the heap where the queue elements are stored, \a heapDim
 * 				is the dimension of that heap. Both \a freeData and \a cpyData are pointers to functions that free and copy
 * 				the data stored by the queue.
 */
typedef struct pQueueCDT
{
	queueElemT *heap;
	int heapDim;
	void (*freeData)(void *);
	void *(*cpyData)(void *);
} pQueueCDT;


/**
 * 	\def	swap(type, a, b)
 * 	\brief	swaps \a a with \a b. \a type corresponds to the type of variable to swap
 */
#define swap(type, a, b)		\
	type temp;					\
	temp=a;						\
	a=b;						\
	b=temp;



/*
 * for debugging purposes-only
 */
void
printHeap(pQueueADT pQueue, char *func)
{
	int i;
	str_debug(PHEAP_DEBUG, func, "In function:  ", TRUE);
	for(i=0; i< pQueue->heapDim; ++i){
	    str_debug(PHEAP_DEBUG, func, "i: ", FALSE);
	    int_debug(PHEAP_DEBUG, func, i, FALSE);
	    str_debug(PHEAP_DEBUG, func, "priority: ", FALSE);
	    int_debug(PHEAP_DEBUG, func, (pQueue->heap+i)->priority, TRUE);
	}
	return;
}

/*
 *
 */

/**
 * 	\fn		static void orderHeap(queueElemT *heap, int shift)
 * 	\brief	Orders the heap so as to keep it in balance after an insertion by
 * 			"bubbling up" the recently inserted value to its correct
 * 			position \n
 *
 * 			\param	heap is a pointer to the heap to order
 * 			\param	shift is the position of the element to bubble up
 * 			Use:
 * 			\code
 * 					bHeapADT b;
 * 					orderHeap(b->heap, b->heapDim-1);
 * 			\endcode
 * \pre		\a heap should be a valid heap pointer
 * \pre		\a shift should be equal to the heap dimension - 1 so as to order the
 * 			entire heap
 * \post	the heap is correctly balanced
 */
static
void
orderHeap(queueElemT *heap, int shift)
{
	queueElemT *lastElem, *father;

	if( shift <= 0  )
		return;
	lastElem=heap+shift;
	father=heap + (shift -1) / 2 ;
	if( lastElem->priority < father->priority )
	{
		swap(queueElemT, *lastElem, *father);
		orderHeap(heap, ( shift - 1 ) / 2);
	}
	return;
}

/**
 * 	\fn 	static void siftDown(queueElemT *heap, int shift, int dim)
 *	\brief	Orders the heap so as to keep it in balance once the root has been
 *			dequeued. \n
 *
 *			Use:
 *			\code
 *					pQueueADT b;
 *					siftDown(b->heap, b->heapDim);
 *			\endcode
 *			\param	heap is a pointer to the heap to order
 *			\param	dim is the dimension of the heap
 *	\pre	\a heap should be a valid heap of dimension \a dim
 *	\post	the heap is ordered
 *
 */
static
void
siftDown(queueElemT *heap, int dim)
{
		queueElemT  cur, lSon, rSon, aux;
		int offset = 0, state = 1;

		if( 2*offset+1 >= dim )
			return;
		cur = heap[offset];
		lSon = heap[2*offset+1];
		if( 2*offset+2 < dim )
		{
			for ( rSon = heap[2*offset+2]; 2*offset+2 <= dim - 1/**/; cur = heap[offset], lSon = heap[2*offset+1], rSon = heap[2*offset+2])
			{
				if ( (cur.priority <= lSon.priority) && (cur.priority <= rSon.priority) )
				{
					state = 0;
					break;
				}
				if ( lSon.priority <= rSon.priority)
				{
					offset = 2*offset + 1;
					aux = lSon;
				}
				else
				{
					offset = 2*offset + 2;
					aux = rSon;
				}
				heap[offset] = cur;
				heap[(offset-1)/2] = aux;
				if( 2*offset+1 >= dim )
					break;
				if(  2*offset+2 >= dim )
				{
					lSon=heap[2*offset+1];
					break;
				}
			}
		}
		if ( (2*offset+1 <= dim - 1) && state)
		{
			if( cur.priority > lSon.priority )
			{
				heap[2*offset+1] = cur;
				heap[offset] = lSon;
			}
		}
		return;
}
/*
 * Public functions
 * ================
 *
 */

/*
 * 	\fn		pQueueADT newPQueue(void (*freeData)(void *), void *(*cpyData)(void *))
 * 	\brief	Creates a new priority queue, \a freeData and \a cpyData are function pointers for
 * 			freeing and copying the data in use in the priority queue
 *
 * 	\pre	\a freeData and \a cpyData should be valid function pointers
 * 	\return	a heap or \c NULL if some error was encountered
 *
 */
pQueueADT
newPQueue(void (*freeData)(void *), void *(*cpyData)(void *))
{
	pQueueADT ret;
	if( (ret=kmalloc(sizeof(pQueueCDT))) != NULL )
	{
		ret->heap=NULL;
		ret->heapDim=0;
		ret->freeData=freeData;
		ret->cpyData=cpyData;
	}
	return ret;
}

/*
 * 	\fn		void freePQueue(pQueueADT pQueue)
 * 	\brief	frees the priority queue passed as parameter
 * 	\pre	\a pQueue should correspond to a valid priority queue
 * 	\post	all the memory used by the queue is freed
 */
void
freePQueue(pQueueADT pQueue)
{
	int i;
	for(i=0; i<pQueue->heapDim; ++i)
		pQueue->freeData( (*(pQueue->heap+i)).data );
	if( pQueue->heap != NULL )
		kfree(pQueue->heap);
	kfree(pQueue);
	return;
}

/*
 * 	\fn		int pEnqueueElem(pQueueADT pQueue, void *data, float priority)
 * 	\brief	Enqueues the \a data passed as parameter in a position of the
 * 			\a pQueue depending on its \a priority. Low priorities will be
 * 			dequeued first.				\n
 * 			Use:
 * 			\code
 * 					pQueueADT pq=newPQueue(freeInt, cpyInt);
 * 					int a=2, b=3, c=2;
 * 					pEnqueueElem(pq, &a, 32.112);
 * 					pEnqueueElem(pq, &b, 32.120);
 * 					pEnqueueElem(pq, &a, 35);
 * 			\endcode
 * 	\pre	\a pQueue should be a valid queue
 * 	\post	the \a data is correctly enqueued in the priority queue depending on its priority
 * 	\return	\c OK or \c ERROR if some error was encountered
 */
int
pEnqueueElem(pQueueADT pQueue, void *data, float priority)
{
	/*
	printHeap(bHeap, "enqueue");
	*/
	int ret=ERROR;
	if( (pQueue->heap=krealloc(pQueue->heap, sizeof(queueElemT)*(pQueue->heapDim+1))) != NULL )
	{
		(*(pQueue->heap+pQueue->heapDim)).priority=priority;
		(*(pQueue->heap+pQueue->heapDim)).data=pQueue->cpyData(data);
		++pQueue->heapDim;
		orderHeap(pQueue->heap, pQueue->heapDim-1);
		ret=OK;
	}
	/*
	printHeap(pQueue, "enqueue");
	*/
	return ret;
}


/*
 * 	\fn		void *pDequeue(pQueueADT pQueue, int *stat)
 * 	\brief	dequeues the first element from the queue \a pQueue and notifies in \a stat if an error has occured by saving \c ERROR
 * 			in it or \c OK otherwise. WARNING: this function returns a cpy that should be freed after use.  \n
 * 			Use:
 * 			\code
 * 					pQueueADT pQueue=newPQueue(freeData, cpyData);
 * 					int a=2, *ret, stat;
 * 					pEnqueue(pQueue, &a);
 * 					ret=pDequeue(pQueue, &stat);
 * 					if( *stat != ERROR )
 * 						printf("%d \n", *ret);
 * 			\endcode
 * 	\pre	\a pQueue should be a valid priority queue
 * 	\return	the element dequeued or \c NULL. The \c NULL value could occurr because the queue is empty or because an error
 * 			occurred. If an error occurs \c ERROR is saved in stat or \c OK otherwise
 */
void *
pDequeue(pQueueADT pQueue, int *stat)
{
	queueElemT newRoot;
	void *retData;
	*stat=OK;
	if( pQueue->heapDim == 0 )
		return NULL;
	/*
	printHeap(pQueue, "dequeue");
	*/
	retData=(*(pQueue->heap)).data;
	newRoot=*(pQueue->heap+ pQueue->heapDim - 1);
	*(pQueue->heap)=newRoot;
	if( pQueue->heapDim > 1 )
	{
		if( (pQueue->heap=krealloc(pQueue->heap, sizeof(queueElemT)*(pQueue->heapDim-1))) == NULL )
		{
			*stat=ERROR;
			return NULL;
		}
		siftDown(pQueue->heap, pQueue->heapDim-1);
	}
	else
		pQueue->heap=NULL;
	--pQueue->heapDim;
	/*
	printHeap(pQueue, "dequeue");
	*/
	return retData;
}

void
*pDequeueIfEqual(pQueueADT pQueue, int (*cmpFn)(void *data, void *data1), void *data1){
    int i, stat;
    for(i=0; i<pQueue->heapDim; ++i){
	   if( cmpFn((pQueue->heap[i]).data, data1) == 0 ){
		  return pDequeue(pQueue, &stat);
	   }
    }
    return NULL;
}

/*
 * 	\fn		int pQueueIsEmpty(pQueueADT pQueue)
 * 	\brief	indicates wether the given priority queue is empty or not
 * 	\pre	\a pQueue should be a valid priority queue
 * 	\return	1 if the given priority queue is empty or 0 otherwise
 */
int
pQueueIsEmpty(pQueueADT pQueue)
{
	return !pQueue->heapDim;
}
