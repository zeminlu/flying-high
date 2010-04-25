/*
 *	semaphore.c
 */

#include "semaphore.h"

typedef struct SEMAPHORE{
	key_t	semid;
	int		semval;
	pid_t	sempid;
	int		semcnt;
	int		qtyProcess;
}SEMAPHORE;