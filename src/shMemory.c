/**
 *	\file shMemory.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#include "shMemory.h"

#define	FREE	1
#define	BUSY	!FREE

static int freeShMems = MAX_SHMEMS;

typedef struct shMem{
	int				shmid;
	key_t			key;
	int 			shMemSize;
	frame_t			*shMemFrame;
	unsigned int	shMemP;
	key_t 			semid;
	int				state;
	int				permFlags;
	int				appnProcessesAmm;
	pid_t			appnProcessesPids[MAX_SHMAPPN];
}shMem_t;

static shMem_t shMemories[MAX_SHMEMS];

int shmget(key_t key, int size){
	return (int)int80(_SYS_SHMGET, (void *)key, (void *)size, NULL);
}

char * shmat(int shmid, key_t *semid){
	return (char *)int80(_SYS_SHMAT, (void *)shmid, (void *)semid, NULL);
}

int shmdt(int shmid){
	return (int)int80(_SYS_SHM_DETACH, (void *)shmid, NULL, NULL);
}

static void shmDestroy(int shmid){
	if (shMemories[shmid].state != FREE){
		return;
	}
	shMemories[shmid].state = FREE;
	shMemories[shmid].key = -1;
	setFramePresence(shMemories[shmid].shMemFrame, FALSE);
	freeFrame(shMemories[shmid].shMemFrame);
	shMemories[shmid].shMemP = 0;
	shMemories[shmid].shMemSize = 0;
	sem_free(shMemories[shmid].semid);
	shMemories[shmid].semid = -1;
	++freeShMems;
	
	return;
}

void initializeShMems(){
	int i, j;
	
	for (i = 0 ; i < MAX_SHMEMS ; ++i){
		shMemories[i].shmid = i;
		shMemories[i].key = -1;
		shMemories[i].shMemSize = 0;
		shMemories[i].shMemFrame = NULL;
		shMemories[i].shMemP = 0;
		shMemories[i].semid = -1;
		shMemories[i].state = FREE;
		shMemories[i].permFlags = (_READ | _WRITE);
		shMemories[i].appnProcessesAmm = 0;
		for (j = 0 ; j < MAX_SHMAPPN ; ++j){
			shMemories[i].appnProcessesPids[j] = -1;
		}
	}
}

int _sys_shmget(key_t key, int size){
	int i, freePos = -1;
	
	if (!freeShMems || size > PAGE_SIZE * PAGES_PER_FRAME){
		/*puts("El tamaño es mayor al de una pagina");*/
		return -1;
	}
	
	for (i = 0 ; i < MAX_SHMEMS ; ++i){
		if (shMemories[i].key == key){
			/*puts("La key es igual");*/
			return -1;
		}
		if (shMemories[i].state == FREE && freePos == -1){
			freePos = shMemories[i].shmid;
		}
	}
	
	if (freePos == -1){
		/*puts("freepos == -1");*/
		return -1;
	}
	
	shMemories[freePos].state = BUSY;
	shMemories[freePos].key = key;
	shMemories[freePos].shMemSize = PAGE_SIZE * PAGES_PER_FRAME;
	if((shMemories[freePos].shMemFrame = getFrame()) == NULL){
		/*puts("Pidiendo el frame");*/
		return -1;
	}
	
	setFramePresence(shMemories[freePos].shMemFrame, TRUE);
	
	shMemories[freePos].shMemP = shMemories[freePos].shMemFrame->address;
		
	if ((shMemories[freePos].semid = sem_get(BLOCK)) == -1){
		/*puts("creando semaforo\n");*/
		return -1;
	}
	
	--freeShMems;
	
	return freePos;
}

char * _sys_shmat(int shmid, key_t *semid){
	int i;
	pid_t pid;
	
	pid = getpid();
	if (shmid >= MAX_SHMEMS || shMemories[shmid].state != BUSY || shMemories[shmid].key == -1 || shMemories[shmid].appnProcessesAmm >= MAX_SHMAPPN){
		return NULL;
	}
	
	++shMemories[shmid].appnProcessesAmm;
	for (i = 0 ; i < MAX_SHMAPPN ; ++i){
		if (shMemories[shmid].appnProcessesPids[i] == -1){
			shMemories[shmid].appnProcessesPids[i] = pid;
			break;
		}
	}
	if (i == MAX_SHMAPPN){
		return NULL;
	}
	
	*semid = shMemories[shmid].semid;
	
	return (char *)shMemories[shmid].shMemP;
}

int _sys_shm_detach(int shmid){
	int i;
	pid_t pid;
	
	pid = getpid();
	if (shmid < 0 || shmid >= MAX_SHMEMS){
		return -1;
	}
	
	for (i = 0 ; i < MAX_SHMAPPN ; ++i){
		if (shMemories[shmid].appnProcessesPids[i] == pid){
			shMemories[shmid].appnProcessesPids[i] = -1;
			--shMemories[shmid].appnProcessesAmm;
			break;
		}
	}
	if (i == MAX_SHMAPPN){
		return -1;
	}
	if (!(shMemories[shmid].appnProcessesAmm)){
		shmDestroy(shmid);
	}
	
	return shMemories[shmid].appnProcessesAmm;
}