/**
 *	pQueue.h
 */

#ifndef _PQUEUE_
#define _PQUEUE_ 	

#include "string.h"
#include "stdio.h"
#include "sysProcess.h"
/*
 *	Struct Section
 */

typedef struct pQueueCDT * pQueueADT;

/*
 *	Function Sections
 */

/**
 *	\fn	pQueueADT newPQueue ( void (*freeFn)(void *), void * (*cpyFn)(void *) );
 *
 *	\brief	This function return a new priority Queue. It creates
 *			a priority queue for _MAX_PRIORITY_ priorities. It receives
 *			the function to copy an element and free an element.
 *
 *	\code
 *		
 *		pQueue * queue;
 *		queue = newPQueue();
 *	
 *	\endcode
 */

pQueueADT newPQueue( void (*freeFn)(void *), void * (*cpyFn)(void *) );

/**
 *	\fn		void freePQueue ( pQueueADT queue );
 *
 *	\brief	This function frees the whole pQueue.
 *
 *	\code
 *			...
 *			freePQueue( queue );
 *			...
 *	\endcode
 */

void freePQueue( pQueueADT queue );

/**
 *	\fn	int push ( pQueueADT queue, void * data, int priority );
 *
 *	\brief 	This function push the element in the correct
 *			priority node and it will be added at last of
 *			the list.
 *	\code
 *			pQueue * queue;
 *			int data = 9;
 *			queue = newPQueue();
 *			...
 *			push(queue, (void*)(&data) );
 *	\endcode
 */

int push( pQueueADT queue, void* data, int priority );

/**
 *	\fn	void * pop ( pQueueADT queue );
 *
 *	\brief	This function return the first element of the queue.
 *			After you call to pop, if the proccess doesn't finish
 *			you must push again the control block, if not you don't
 *			do anything.
 *	\code
 *			pQueue * queue;
 *			int	data = 10, ret;
 *
 *			queue = newPQueue();
 *			push( (void*)(&data) );
 *			ret = *((int *)pop(queue));
 *	\endcode
 */

void * pop( pQueueADT queue );

#endif
