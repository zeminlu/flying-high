/**
 *	\file schedule.c
 *
 *		\brief The OS scheduler module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
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

extern process_t *runningProcess;

extern int qtyProccessTable;

static int Algorithim = RPG;

static int prevPro = 0;

/*
 *	Functions Section
 */

static process_t * roundRobinSchedule()
{
	static int lastTask;
	int i;
	
	lastTask = (runningProcess == NULL) ? -1 : runningProcess->pid;
	i = (lastTask + 1 == MAX_PROCESS ? 1 : lastTask + 1);
	while(i != lastTask)
	{
		if( processTable[i].pid != -1 && processTable[i].state == READY )
		{		
			return &processTable[i];
		}
		
		i = (i + 1 == MAX_PROCESS ? 0 : i + 1);
	}
	return initProcess;
}

process_t * rpgSchedule()
{
	int procPos;
	
	procPos = checkWhatAreReady();
	increaseRPGCounter();
	processTable[procPos].rpgPrior = 0;
	/*processTable[procPos].rpgOld = 0;*/
	prevPro = procPos;
	return &processTable[procPos];			/* If there isn't a process to be excecuted it return Idle(0) process */
}

process_t * getNextTask()
{	
	process_t * proc;
	/*char *video = (char*)0xb8002;*/
	
	proc = (Algorithim == ROUND_ROBIN ? roundRobinSchedule() : rpgSchedule());
	/*while(1){
		*video = (proc->pid + 30);
		*(video + 18) = 'A';
	}*/
	return proc;
}

int checkWhatAreReady( void )
{
	int i, proc;
	
	for( i = 1, proc = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].state == READY && processTable[i].rpgPrior >= MAX_RPG && processTable[i].priority > processTable[proc].priority )
		{
			if( i != prevPro )
				proc = i;
		}
	}
	return proc;
}

void increaseRPGCounter()
{
	int i;
	
	for( i = 1 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].state == READY )
		{
			processTable[i].rpgPrior += evaluate(processTable[i].priority);
			processTable[i].rpgOld++;
		}
	}
}

void changeAlgorithimSchedule( void ){
	Algorithim = (Algorithim == ROUND_ROBIN ? RPG : ROUND_ROBIN);
	if( Algorithim == ROUND_ROBIN )
		puts("Using Round Robin scheduler...\n");
	else	
		puts("Using RPG scheduler...\n");
}


