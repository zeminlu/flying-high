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

extern process_t *runningProcess;

extern process_t *initProcess;

extern process_t *nextProcess;

extern pid_t nextPID = 1;

extern sysTTY ttyTable;

extern int qtyProccessTable;

/*
 *	Functions Section
 */

proccess_t * getNextTask()
{	
	proccess_t * proc;
	int procPos;
	
	procPos = getNextProccess();
	proc = &proccessTable[proc];
	if( proc == 0 )
		return initProcess;
	/*currentPID = proc->pid;
	runningProcess = proc;
	*/
	nextProcess = proc;
	return proc;
}

int getNextProccess()
{	
	int i;
	
	if( proccessTableIsEmpty() )
		return 0;
	else
	{
		
	}
}

int proccessTableIsEmpty()
{
	int i;
	
	if( qtyProccessTable == 0 )
		return 1;
	for( i = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( proccessTable[i].state != RUNNING )
			return 0;
	}
	return 1;
}