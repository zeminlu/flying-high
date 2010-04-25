/*
 *	shMemory.c
 */

#include "shMemory.h"

#define	FREE	1
#define	BUSSY	!FREE

typedef struct shMem{
	int		shmid;
	key_t	key;
	int 	shMemSize;
	char *	shMemP;
	SEMAPHORE *	sempahore;
	int		state;
	int		permFlags;
}shMem;
