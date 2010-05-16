/*
 *	semaphore.h
 */
#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "types.h"
#include "process.h"

#define BLOCK 0
#define WAIT 1
#define MAX_SEMS 64
#define UNUSED 0
#define USED !UNUSED

key_t sem_get(int mode);

void sem_free( key_t semid );

void sem_wait( key_t sem );

void sem_signal( key_t sem );

void initializeSemaphores();

#endif