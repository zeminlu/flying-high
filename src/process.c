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

tty_t getCurrentTTY()
{
	return runningProcess->tty;
}

int getProccessByPid( pid_t pid )
{
	int i;
	
	/*if( pid == 0 )
	{
		return initProcess;
	}*/
	for( i = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].pid == pid )
			return i;
	}
	return -1;
}

int getProccessByName( char * name )
{
	int i;
	
	if( !strcmp(name, "Idle") )
	{
		return -1;
	}
	for( i = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( !strcmp(processTable[i].name, name) )
			return i;
	}
	return -1;
}

