/**
 *	\file pQueueP.c
 *
 *		\brief The priority queue module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
/*
 *	Includes Section
 */

#include "../inc/pQueueP.h"

/*
 *	Struct Section
 */

typedef struct Node_t{
	void *data;
	struct Node_t * next;
	}Node_t;

typedef struct listWithPrio{
	int priority;		/* The priority */
	int pQueueDim;		/* The qty of proccess in the pQueue */
	Node_t * first;		/* The pointer to the first node */
	Node_t * last;		/* The pointer to the last node, it will cause the insertion more efficient */
	}listWithPrio;

typedef struct pQueueCDT{
	listWithPrio * queue;
	void * 	(*cpyFn)(void *);
	void	(*freeFn)(void *);
	}pQueueCDT;

/*
 *	Main Program
 */

void * copyNum( void * num )
{
	return num;
}

void freeNode ( void * node)
{
	free(node);
}

void printQueue( pQueueADT queueADT )
{
	int i;
	Node_t *cur;
	
	for( i = 0 ; i < MAX_PRIORITIES ; ++i )
	{
		if( (queueADT->queue)[i].pQueueDim > 0 )
		{
			puts("pQueueDim: %d\n", (queueADT->queue)[i].pQueueDim);
			cur = (queueADT->queue)[i].first;
			while( cur != NULL )
			{
				puts("data: %d\n", *((int*)(cur->data)) );
				cur = cur->next;
			}
		}
	}
}

pQueueADT newPQueue( void (*freeFnP)(void *), void * (*cpyFnP)(void *) )
{
	pQueueCDT * queueADT;
	int i;
	
	if( (queueADT = malloc(sizeof(pQueueCDT))) == NULL )
	{
		puts("Error creating the queue\n");
		return NULL;
	}
	queueADT->cpyFn = cpyFnP;
	queueADT->freeFn = freeFnP;
	if( (queueADT->queue = malloc( sizeof(listWithPrio) * MAX_PRIORITIES)) == NULL )
	{
		puts("Error creating the priority list\n");
		return NULL;
	}
	for( i = 0 ; i < _MAX_PRIORITY_ ; ++i )
	{
		(queueADT->queue)[i].priority = i;
		(queueADT->queue)[i].pQueueDim = 0;
		(queueADT->queue)[i].first = NULL;
		(queueADT->queue)[i].last = NULL;
	}
	return queueADT;
}

void freePQueue( pQueueADT queueADT )
{
	int i;
	Node_t *cur, *prev;
	
	for( i = 0 ; i < MAX_PRIORITIES ; ++i )
	{
		if( (queueADT->queue)[i].pQueueDim > 0 )
		{	
			cur = (queueADT->queue)[i].first;
			while( cur != NULL )
			{
				prev = cur;
				free(cur);
				cur = prev->next;
			}
		}
	}
	free( (queueADT->queue) );
	free(queueADT);
}

int enque( pQueueADT queue, void* data, int prio )
{
	
	if( (queue->queue)[prio].pQueueDim == 0 )
	{
		if( ((queue->queue)[prio].first = (queue->queue)[prio].last = malloc( sizeof(Node_t))) == NULL )
		{
			return 1;
		}
		((queue->queue)[prio].first)->data = queue->cpyFn(data);
		((queue->queue)[prio].first)->next = NULL;
	}else
	{
		if( (((queue->queue)[prio].last)->next = malloc(sizeof(Node_t))) == NULL )
		{
			return 1;
		}
		(((queue->queue)[prio].last)->next)->data = queue->cpyFn(data);
		(((queue->queue)[prio].last)->next)->next = NULL;
		(queue->queue)[prio].last = (((queue->queue)[prio].last)->next);
	}
	(queue->queue)[prio].pQueueDim++;
	
	return 0;
}

void * deque( pQueueADT queue )
{
	int i;
	void * cpyData = NULL;
	Node_t * aux;
	
	for( i = 0 ; i < MAX_PRIORITIES ; ++i )
	{
		if( (queue->queue)[i].pQueueDim > 0 )
		{
			aux = (queue->queue)[i].first;
			cpyData = queue->cpyFn( (queue->queue)[i].first->data );
			(queue->queue)[i].first = ((queue->queue)[i].first)->next;
			queue->freeFn((void*)aux);
			--(queue->queue)[i].pQueueDim;
			return cpyData;
		}
	}
	return NULL;
}

int isEmpty( pQueueADT queue)
{
	int i,qty = 0;
	
	for( i = 0 ; i < MAX_PRIORITIES ; ++i )
	{
		if( (queue->queue)[i].pQueueDim > 0 )
			++qty;
	}
	
	return qty;
}