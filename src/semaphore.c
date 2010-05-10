/*
 *	semaphore.c
 */

#include "semaphore.h"

typedef struct semQueue{
	pid_t	pids[MAX_PROCESS];
	int		actPos;
	int		pidsQty;
}semQueue_t;

typedef struct semaphore{
	key_t		semid;
	int			semval;
	int			state;
	pid_t		sempid;
	semQueue_t	waitQueue;
}semaphore_t;

static semQueue_t queues[MAX_SEMS];

static semaphore_t sems[MAX_SEMS];

static int semIsEmpty(semQueue_t queue){
	return !queue.pidsQty;
}

static int semEnque(semQueue_t queue){
	if (queue.pidsQty == MAX_PROCESS){
		return -1;
	}
	queue.pids[(queue.actPos + queue.pidsQty) % MAX_SEMS] = getpid();
	
	return ++queue.pidsQty;
}

static pid_t semDeque(semQueue_t queue){
	if (queue.pidsQty == 0){
		return -1;
	}
	--queue.pidsQty;
	
	return queue.pids[queue.actPos = (queue.actPos++ % MAX_SEMS)];	
}

void initializeSemaphores(){
	int i;
	
	for (i = 0 ; i < MAX_SEMS ; ++i){
		sems[i].semid = i;
		sems[i].state = UNUSED;
		sems[i].sempid = -1;
		sems[i].semval = 0;
		sems[i].waitQueue = queues[i];
		sems[i].waitQueue.actPos = 0;
		sems[i].waitQueue.pidsQty = 0;
	}
	
	return;
}

key_t sem_get(){
	return (key_t)int80(_SYS_SEM_GET, NULL, NULL, NULL);
}

void sem_free( key_t semid ){
	int80(_SYS_SEM_FREE, (void *)semid, NULL, NULL);
	return;
}

void sem_wait( key_t sem, int count ){
	int80(_SYS_SEM_WAIT, (void *)sem, (void *)count, NULL);
	return;
}

void sem_signal( key_t sem, int count ){
	int80(_SYS_SEM_SIGNAL, (void *)sem, (void *)count, NULL);
	return;
}

key_t _sys_sem_get(){
	int i;
	
	for (i = 0 ; i < MAX_SEMS ; ++i){
		if (sems[i].state == UNUSED){
			break;
		}
	}
	
	if (i == MAX_SEMS){
		return -1;
	}
	
	sems[i].state = USED;
	sems[i].sempid = getpid();
		
	return sems[i].semid;
}

void _sys_sem_free( key_t semid ){
	if (sems[semid].state != UNUSED){
		return;
	}
	
	sems[semid].semval = 0;
	sems[semid].state = UNUSED;
	sems[semid].sempid = -1;
	sems[semid].waitQueue.actPos = 0;
	sems[semid].waitQueue.pidsQty = 0;
	
	return;
}

int _sys_sem_wait(key_t semid){
	
	if (sems[semid].semval != 0){
		if (semEnque(sems[semid].waitQueue) == -1){
			return -1;
		}
		sysSelfBlock();
	}	
	++sems[semid].semval;
	
	return 0;
}

int _sys_sem_signal(key_t semid){
	if (sems[semid].semval != 1){
		return -1;
	}
	
	if (!semIsEmpty(sems[semid].waitQueue)){
		sysUnblock(semDeque(sems[semid].waitQueue));
	}
	
	return --sems[semid].semval;
}