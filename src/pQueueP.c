/**
 *	\file	pQueue.c
 */

/*
 *	Includes Section
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

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
	
	for( i = 0 ; i < _MAX_PRIORITY_ ; ++i )
	{
		if( (queueADT->queue)[i].pQueueDim > 0 )
		{
			printf("pQueueDim: %d\n", (queueADT->queue)[i].pQueueDim);
			cur = (queueADT->queue)[i].first;
			while( cur != NULL )
			{
				printf("data: %d\n", *((int*)(cur->data)) );
				cur = cur->next;
			}
		}
	}
}

/*int main (int argc, char const *argv[])
{
	pQueueADT	queue;
	int num1 = 1, num2 = 2, num3 = 3, ret1,ret2,ret3;
	
	queue= newPQueue(freeNode, copyNum);
	push(queue, (void*)(&num1), 0);
	push(queue, (void*)(&num3), 3);
	
	printQueue(queue);
	ret1 = *((int*)pop(queue));
	push(queue, (void*)(&num2), 2);
	
	ret2 = *((int*)pop(queue));
	ret3 = *((int*)pop(queue));
	
	printf("num1: %d num2: %d num3: %d\n", ret1, ret2, ret3);
	freePQueue(queue);
	return 0;
}*/


pQueueADT newPQueue( void (*freeFnP)(void *), void * (*cpyFnP)(void *) )
{
	pQueueCDT * queueADT;
	int i;
	
	if( (queueADT = malloc(sizeof(pQueueCDT))) == NULL )
	{
		printf("Error en la creacion de la queue");
		return NULL;
	}
	queueADT->cpyFn = cpyFnP;
	queueADT->freeFn = freeFnP;
	if( (queueADT->queue = malloc( sizeof(listWithPrio) * _MAX_PRIORITY_)) == NULL )
	{
		printf("Error en la creacion de la lista con las prioridades");
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
	
	for( i = 0 ; i < _MAX_PRIORITY_ ; ++i )
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

int push( pQueueADT queue, void* data, int prio )
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

void * pop( pQueueADT queue )
{
	int i;
	void * cpyData = NULL;
	Node_t * aux;
	
	for( i = 0 ; i < _MAX_PRIORITY_ ; ++i )
	{
		if( (queue->queue)[i].pQueueDim > 0 )
		{
			aux = (queue->queue)[i].first;
			cpyData = queue->cpyFn( (queue->queue)[i].first->data );
			(queue->queue)[i].first = ((queue->queue)[i].first)->next;
			queue->freeFn((void*)aux);
			/*printf("cpyData: %d pQueueDim: %d\n", *((int*)cpyData), (queue->queue)[i].pQueueDim );*/
			--(queue->queue)[i].pQueueDim;
			return cpyData;
		}
	}
	return NULL;
}