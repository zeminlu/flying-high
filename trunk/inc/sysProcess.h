#ifndef _SYSPROCESS_H_
#define _SYSPROCESS_H_

#include "keyboard_buffer.h"
#include "config.h"
#include "types.h"

#define	ATOMIC				1
#define UNATOMIC			!ATOMIC
#define RUNNING				0
#define READY				1
#define BLOCKED				2
#define WAITING_CHILD		3
#define WAITING_TIME		4
#define WAITING_PID			5
#define TERMINATED			6
#define ZOMBIE				7
#define	FOREGROUND			0
#define	BACKGROUND			1


typedef void (pfunc_t)(int);

/*enum processErrorList{
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
*/
typedef struct process_t {
	pid_t pid;
	pid_t ppid;
	pid_t gid;
	tty_t tty;
	pid_t childs[MAX_CHILDS];
	int childsQty;
	char name[MAX_PROCESS_NAME + 1];
	int state;
	int rpgPrior;
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
