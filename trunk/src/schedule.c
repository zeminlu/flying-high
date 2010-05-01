/*
 *	schedule.c
 */

/*
 *	System Includes
 */

#include "schedule.h"

/*
 *	Extern and local variables
 */

extern process_t processTable[MAX_PROCESS];

extern process_t *nextProcess;

extern process_t *initProcess;

extern int qtyProccessTable;

/*
 *	Functions Section
 */

static process_t * roundRobinSchedule()
{
	static int lastTask = 0;
	int i;
	
	i = lastTask + 1;
	while(i != lastTask)
	{
		if( processTable[i].pid != -1 && processTable[i].state == READY )
		{		
			lastTask = i;
			return &processTable[i];
		}
		
		++i;
		
		if( i == MAX_PROCESS ){
			i = 0;
		}
	}
	return initProcess;
}

process_t * rpgSchedule()
{
	int i, procPos;
	process_t *auxReady[MAX_PROCESS];
	
	for( i = 1 ; i < MAX_PROCESS ; ++i )
		auxReady[i] = NULL;
	
	checkWhatAreReady(auxReady);
	procPos = getTheOlder( auxReady );
	increaseRPGCounter();
	processTable[procPos].tickCounter = 0;
	processTable[procPos].rpgPrior = 0;
	return &processTable[procPos];			/* If there isn't a process to be excecuted it return Idle(0) process */
}

process_t * getNextTask()
{	
	process_t * proc;
	/*char *video = (char*)0xb8002;*/
	
	proc = (ALGORITHIM == ROUND_ROBIN ? roundRobinSchedule() : rpgSchedule());
	/*while(1){
		*video = (proc->pid + 30);
		*(video + 18) = 'A';
	}*/
	return proc;
}

void checkWhatAreReady( process_t * pro[] )
{
	int i, k;
	
	for( i = 1, k = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].state == READY && processTable[i].rpgPrior >= MAX_RPG )
			*(pro[k++]) = processTable[i];
	}
}

int getTheOlder( process_t * pro[] )
{
	int i = 0, older = 0;
	
	while( pro[i] != NULL )
	{
		if( pro[i+1] != NULL )
		{
			if( pro[i]->priority > pro[i+1]->priority && pro[i]->priority > pro[older]->priority )
			{
				older = i;
			}else if( pro[i]->priority < pro[i+1]->priority && pro[i+1]->priority > pro[older]->priority )
			{
				older = (i + 1);
			}else
			{
				if( pro[i]->tickCounter >= pro[i+1]->tickCounter && pro[i]->tickCounter > pro[older]->tickCounter )
				{
					older = i;
				}else if( pro[i]->tickCounter < pro[i+1]->tickCounter && pro[i+1]->tickCounter > pro[older]->tickCounter )
				{
					older = (i + 1);
				}
			}
		}
		++i;
	}
	return older;
}

void increaseRPGCounter()
{
	int i;
	
	for( i = 1 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].state == READY )
		{
			processTable[i].rpgPrior += evaluate(processTable[i].priority);
			processTable[i].tickCounter = 1;
		}
	}
}

void setupScheduler( void )
{
	/*int i;*/
	
	initProcess->pid = 0;
	initProcess->level = FOREGROUND;
	initProcess->priority = 4;
	initProcess->ppid = 0;
	initProcess->gid = 0;
	initProcess->tty = 0;
	initProcess->childsQty = 0;
	memcpy(initProcess->name, "Idle", 5);
	initProcess->state = RUNNING;
	initProcess->stack = malloc(0x200);
	/*createStackFrame(initProcess, );*/
}


