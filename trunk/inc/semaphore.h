/*
 *	semaphore.h
 */

#include "types.h"

SEMAPHORE * sem_get( int value );

void sem_free( key_t semid );

void sem_wait( SEMAPHORE * sem, int count );

void sem_signal( SEMAPHORE * sem, int count );

int sem_getid( SEMAPHORE * sem );