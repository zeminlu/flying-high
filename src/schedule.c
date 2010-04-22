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

extern sysTTY ttyTable;

extern int qtyProccessTable;

/*
 *	Functions Section
 */

process_t * getNextTask()
{	
	process_t * proc;
	
	proc = ALGORITHIM == ROUND_ROBIN ? roundRobinSchedule() : rpgSchedule();
	nextProcess = proc;
	return proc;
}

process_t * roundRobinSchedule()
{
	static int lastTask = 0;
	int i;
	
	if( lastTask == MAX_PROCESS )
		lastTask = 2;
	for( i = lastTask + 1 ; i != lastTask ; ++i )
	{
		if( i == MAX_PROCESS )
			i = 2;
		if( processTable[i].state == READY && processTable[i].state != RUNNING )
		{
			lastTask = i;
			return &processTable[i];
		}
	}
	return &processTable[0];
	
}

process_t * rpgSchedule()
{
	int i, procPos;
	process_t *auxReady[MAX_PROCESS];
	
	for( i = 0 ; i < MAX_PROCESS ; ++i )
		auxReady[i] = NULL;
	
	checkWhatAreReady(auxReady);
	procPos = getTheOlder( auxReady );
	increaseRPGCounter();
	processTable[procPos].tickCounter = 0;
	processTable[procPos].rpgPrior = 0;
	return &processTable[procPos];			/* If there isn't a process to be excecuted it return Idle(0) process */
}

void checkWhatAreReady( process_t * pro[] )
{
	int i, k;
	
	for( i = 2, k = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].state == READY && processTable[i].state != RUNNING && processTable[i].rpgPrior >= MAX_RPG )
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
	
	for( i = 2 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].state == READY && processTable[i].state != RUNNING )
		{
			processTable[i].rpgPrior += evaluate(processTable[i].priority);
			processTable[i].tickCounter = 1;
		}
	}
}


