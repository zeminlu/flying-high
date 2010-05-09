/*
 *	semaphore.h
 */

#include "types.h"

#define MAX_SEMS 64
#define FREE 0
#define USED !FREE

key_t sem_get( int value );

void sem_free( key_t semid );

void sem_wait( key_t sem, int count );

void sem_signal( key_t sem, int count );