#include "process.h"

extern process_t processTable[MAX_PROCESS];

extern process_t *runningProcess;

extern process_t *initProcess;

void * memmap(){
	return (void *)int80(_SYS_MEMMAP, NULL, NULL, NULL);
}

pid_t getpid()
{
	return runningProcess->pid;
}

pid_t getppid()
{
	return runningProcess->ppid;
}

tty_y getCurrentTTY()
{
	return runningProcess->tty;
}

proccess_t * getProccessByPid( pid_t pid )
{
	int i;
	
	/*if( pid == 0 )
	{
		return initProcess;
	}*/
	for( i = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].pid == pid )
			return &processTable[i];
	}
	return NULL;
}

proccess_t * getProccessByName( char * name )
{
	int i;
	
	if( !strcmp(name, "Idle") )
	{
		return initProcess;
	}
	for( i = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( !strcmp(processTable[i].name, name) )
			return &processTable[i];
	}
	return NULL;
}

