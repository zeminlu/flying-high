/*
 *	shMemory.c
 */

#include "shMemory.h"

#define	FREE	1
#define	BUSY	!FREE

static int freeShMems = MAX_SHMEMS;

typedef struct shMem{
	int			shmid;
	key_t		key;
	int 		shMemSize;
	frame_t		*shMemFrame;
	char *		shMemP;
	key_t 		semid;
	int			state;
	int			permFlags;
	int			appnProcessesAmm;
	pid_t		appnProcessesPids[MAX_SHMAPPN];
}shMem_t;

static shMem_t shMemories[MAX_SHMEMS];

int shmget(key_t key, int size){
	int i, freePos;
	
	if (!freeShMems || size > PAGE_SIZE * PAGES_PER_FRAME){
		return -1;
	}
	
	for (i = 0 ; i < MAX_SHMEMS ; ++i){
		if (shMemories[i].key == key){
			return -1;
		}
		if (shMemories[i].shmid == -1 && freePos == -1){
			freePos = shMemories[i].shmid;
		}
	}
	
	if (i >= MAX_SHMEMS){
		return -1;
	}
	
	shMemories[freePos].state = BUSY;
	shMemories[freePos].key = key;
	shMemories[freePos].shMemSize = PAGE_SIZE * PAGES_PER_FRAME;
	if((shMemories[freePos].shMemFrame = getFrame()) == NULL){
		return -1;
	}
	
	setFramePresence(shMemories[shmid].shMemFrame, TRUE);
	
	shMemories[freePos].shMemP = shMemories[freePos].shMemFrame->address;
	
	if ((shMemories[freePos].semid = sem_get()) == -1){
		return -1;
	}
	
	--freeShMems;
	
	return freePos;
}

char * shmat(int shmid){
	int i;
	
	if (shmid >= MAX_SHMEMS || shMemories[shmid].state != BUSY || shMemories[shmid].key == -1 || shMemories[shmid].appnProcessesAmm >= MAX_SHMAPPN){
		return NULL;
	}
	++shMemories[shmid].appnProcessesAmm;
	for (i = 0 ; i < MAX_SHMAPPN ; ++i){
		if (shMemories[shmid].appnProcessesPids[i] == -1){
			shMemories[shmid].appnProcessesPids[i] = getpid();
			break;
		}
	}
	if (i == MAX_SHMAPPN){
		return NULL;
	}
	
	return shMemories[shmid].shMemP;
}

static void shmDestroy(int shmid){
	if (shMemories[shmid].state != FREE){
		return;
	}
	shMemories[shmid].state = FREE;
	shMemories[shmid].key = -1;
	setFramePresence(shMemories[shmid].shMemFrame, FALSE);
	freeFrame(shMemories[shmid].shMemFrame);
	shMemories[shmid].shMemP = NULL;
	shMemories[shmid].size = 0;
	sem_free(shMemories[shmid].semid);
	shMemories[shmid].semid = -1;
	++freeShMems;
	
	return;
}

int shmDetach(int shmid){
	int i;
	
	if (shmid < 0 || shmid >= MAX_SHMEMS){
		return -1;
	}
	
	for (i = 0 ; i < MAX_SHMAPPNPROCESSES ; ++i){
		if (shMemories[shmid].appnProcessesPids == getpid()){
			shMemories[shmid].appnProcessesPids = -1;
			--shMemories[shmid].appnProcessesAmm;
			break;
		}
	}
	if (i == MAX_SHMAPPNPROCESSES){
		return -1;
	}
	if (!shmid[shmid].appnProcessesAmm){
		shmDestroy(shmid);
	}
	
	return shmid[shmid].appnProcessesAmm;
}

void initializeShMems(){
	int i;
	
	for (i = 0 ; i < MAX_SHMEMS ; ++i){
		shMemories[i].shmid = i;
		shMemories[i].key = -1;
		shMemories[i].shMemSize = 0;
		shMemories[i].shMemFrame = NULL;
		shMemories[i].shMemP = NULL;
		shMemories[i].semid = -1;
		shMemories[i].state = FREE;
		shMemories[i].permFlags = (_READ | _WRITE);
	}
}