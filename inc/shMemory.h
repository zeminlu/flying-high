/*
 *	shMemory.h
 */

#ifndef _SHMEMORY_H_
#define _SHMEMORY_H_

#include "semaphore.h"

#define	IPC_RMID	1
#define IPC_SET		2
#define IPC_STAT	3
#define IPC_CREAT	4
#define	IPC_PRIVATE 5

#define MAX_SHMEMS	8
#define MAX_SHMAPPN	MAX_PROCESS

int shmget(key_t key, int size);

char * shmat(int shmid, key_t *semid);

int shmDetach(int shmid);

void initializeShMems();

#endif