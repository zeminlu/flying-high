/*
 *	sysprocess.h
 *
 * 		Defines the system process interface and defines the public
 * 		function that saves and changes the context between two processes.
 * 		All the process-releated system calls implementations, defined in process.h,
 * 		are defined here.
 *
 *		Author: Guido Marucci Blas
 *
 */
#ifndef SYSPROCESS_H_
#define SYSPROCESS_H_
#include "process.h"

#define	ATOMIC				1
#define UNATOMIC			!ATOMIC

#define MAX_FILES			/*20 */ 40
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
	PERROR_INVALID_SIGNAL,
};

enum processStates {RUNNING, READY, BLOCKED, WAITING_CHILD, WAITING_TIME, WAITING_PID, TERMINATED, DEAD};

typedef struct PROCESS {
	pid_t pid;
	pid_t ppid;
	pid_t gid;
	tty_t tty;
	pid_t childs[MAX_CHILDS];
	int childsQty;
	char name[MAX_PROCESS_NAME + 1];
	int state;
	int files[MAX_FILES];
	int openedFiles;
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
} PROCESS;

PROCESS *getProcessTable(void);

/**
 *	@fn initMultitasker(void)
 *
 *		@brief This function initializes the multitasker
 *		and creates the special init process. This function
 *		can only be called once. Other calls will return FALSE.
 *		@param The init process' main function.
 *		@return TRUE or FALSE.
 */
int initMultitasker(pfunc_t init);

/**
 *	@fn void yield(void)
 *
 *		@brief This function makes the calling process to release
 *		the CPU and changes it's state to READY.
 */
void yield(void);

/**
 *	@fn void setProcessAtomicity(pid_t pid, int mode);
 *		
 *		@brief This function changes the process' atomicity identified
 *		by pid.
 *		@param mode ATOMIC or UNATOMIC.
 *
 */
void setProcessAtomicity(pid_t pid, int atomic);

char *getProcessName(pid_t pid);

int *getFileTable(pid_t pid);

void waitTty(tty_t tty);

void signalTty(tty_t tty);

int getProcessState(pid_t pid);

/**
 *	@fn int getOpenedFilesQty(pid_t pid)
 *
 */
int *getOpenedFilesQty(pid_t pid);

/**
 *	@fn void multitasker(void)
 *
 *		@brief This function asks the scheduler
 *		which is the next process that will be put
 *		to run and makes the context change between
 *		the running process and the next process. It
 *		should be called from the timer tick interrupt handler
 */
void multitasker(void);

/**
 *	@fn void multitasker2(void)
 *
 *		@brief This function asks the scheduler
 *		which is the next process that will be put
 *		to run and makes the context change between
 *		the running process and the next process.
 *		This function does not count ticks
 */
extern void multitasker2(void);

/**
 *	@fn const char *getProcessErrorMsg(void)
 *
 *		@brief This function returns a string
 *		that represents the error occured during
 *		the last performed operation.
 */
const char *getProcessErrorMsg(void);

/**
 *	@fn int getProcessErrorCode(void)
 *
 *		@brief This function returs the error
 *		code of the error occured during
 *		the last performed operation.
 */
int getProcessErrorCode(void);

/* System Calls Implementations */

/**
 *	@fn pid_t sysCreateProcess(char *name, pfunc_t main, void *args)
 *		
 *		@brief Creates a new process into the system and sets
 *		it ready to run. The process will be created in unatomic
 *		mode(preemptive mode) and with tty -1.
 *		@param name The process' name
 *		@param main The process' main function. Must Not Be NULL.
 *		@param args A pointer to an argument list.
 *		@return THe new process' pid or -1 in case of error.
 *
 */
pid_t sysCreateProcess(char *name, pfunc_t main, int args, int level);

/**
 *	@fn void sysExit(int status)
 *
 * 		@brief This function terminates the calling process
 * 		and status is returned to whom is waiting for the
 * 		calling process to exit.
 *		@param status The process' exit status.
 *
 */
void sysExit(int status);

/**
 *	@fn int sysKill(int pid, int signal)
 *
 * 		@brief This function sends a signal to any
 * 		process group or process.
 * 		@param pid If pid is positive the signal is sent to
 * 		the process with the ID specified by pid. If pid is zero
 * 		the signal is sent to every process in the process group
 * 		of the calling process.
 * 		@param signal The signal to be send.
 * 		@return On success zero is returned, -1 in case of error.
 *
 */
int sysKill(int pid, int signal);

/**
 *	@fn pid_t sysWait(int *status)
 *
 *		@brief This function is used to wait for
 *		state changes in a child of the calling process and
 *		obtain information about the child whose state changed.
 *		@param status A pointer to where the child process status
 *		will be placed.
 *		@return The child process' pid.
 *
 */
pid_t sysWait(int *status);

/**
 *	@fn pid_t sysWaitpid(pid_t pid, int *status, int options)
 *
 * 		@brief This function suspends execution of the calling process until 
 * 		a child specified by pid argument has changed state.
 *		@param pid The child process' pid.
 *		@param status A pointer to where the child process status
 *		will be placed.
 *		@param options This parameters is ignored. Future Implementation.
 *		@return The child process' pid.
 */
pid_t sysWaitpid(pid_t pid, int *status, int options);

/*
 *	@fn unsigned int sysSleep(unsigned int seconds);
 *
 *		@brief This function makes the calling process
 *		sleep until 'seconds' have elapsed or a signal
 *		arrives which is not ignored.
 *		@param seconds The number of seconds the calling process will sleep.
 *		@return Zero if the requested time has elapsed or the remaining seconds
 *		if the sleep was interrupted by a signal.
 */
unsigned int sysSleep(unsigned int seconds);

/*
 *	@fn pid_t sysGetpid(void)
 *
 *		@brief This function gets the pid of the calling process.
 */
pid_t sysGetpid(void);

/*
 * @fn pid_t sysGetppid(void)
 *
 * 		@brief This function gets the pid of the calling process' parent.
 */
pid_t sysGetppid(void);

/*
 * @fn pid_t sysGetpgrp(void)
 *
 * 		@brief This function gets the gid of the calling process.
 */
pid_t sysGetpgrp(void);

/*
 *	@fn int sysSetpgid(pid_t pid, pid_t pgid)
 *
 *		@brief This function sets the gid of the process identified by 'pid'
 *		with 'pgid'. If 'pid' is set to 0 the calling process' id is used.
 *		If 'pgid' is set to 0 the calling process' gid is used.
 *		@param pid The process' pid.
 *		@param pgid The process' gid.
 *		@return 0 on success or -1 in case of error.
 */
int sysSetpgid(pid_t, pid_t pgid);

int sysSetlevel(int level);

int sysGetlevel();

int sysGetpriority();

int sysSetpriority(pid_t pid, int prio);

unsigned sysTime(void);

int sysDup(int fd1);

int sysDup2(int fd1, int fd2);

/*
 *	@fn tty_t getTty(pid_t pid)
 *
 *		@brief Gets the process' tty.
 *		@param pid The process' pid.
 */
tty_t sysGetTty(pid_t pid);

/*
 *	@fn tty_t setTty(pid_t pid, tty_t tty)
 *		
 *		@brief Sets the process' tty.
 *		@param pid The process' pid.
 *		@param tty The process' tty.
 */
tty_t sysSetTty(pid_t pid, tty_t tty);

#endif
