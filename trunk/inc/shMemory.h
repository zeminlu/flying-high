/*
 *	shMemory.h
 */

#include "semaphore.h"

#define	IPC_RMIND	1
#define IPC_SET		2
#define IPC_STAT	3

int shmget(key_t key, int size, int permflags);

char * shmat( int shmid, const void * daddr, int shmflags );

int	shmctl( int shmid, int command );