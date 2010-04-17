#ifndef _SYSPROCESS_H_
#define _SYSPROCESS_H_

#include "process.h"

#define	ATOMIC				1
#define UNATOMIC			!ATOMIC

#define MAX_PROCESS			24
#define MAX_CHILDS			30
#define MAX_PROCESS_NAME	36
#define MAX_PRIORITIES		4

enum processErrorList{
	PERROR_NO_ERROR, 
	PERROR_BAD_PARAM, 
	PERROR_MAX_PROCESS_REACHED, 
	PERROR_MEMORY_ERROR,
	PERROR_MULTITASK_IS_ON,
	PERROR_INIT_FAILED,
	PERROR_MAX_CHILDS_REACHED,
	PERROR_INVALID_PID,
	PERROR_INVALID_SIGNAL
};

enum processStates {RUNNING, READY, BLOCKED, WAITING_CHILD, WAITING_TIME, WAITING_PID, TERMINATED, DEAD};

typedef struct process {
	pid_t pid;
	pid_t ppid;
	pid_t gid;
	tty_t tty;
	pid_t childs[MAX_CHILDS];
	int childsQty;
	char name[MAX_PROCESS_NAME + 1];
	int state;
	unsigned tickCounter;
	int priority;
	int level;
	pid_t waitingPid;
	pid_t sleepingPid;
	int waitedStatus;
	void *ebp;
	void *esp;
	void *stack;
	void *heap;
	int atomicity;
} process_t;

#endif