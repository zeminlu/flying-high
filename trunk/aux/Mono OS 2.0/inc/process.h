/*
 *	process.h
 *
 *		Defines all the process related system calls
 *
 * 		Author: Guido Marucci Blas
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#define SIGKILL			1

#define FOREGROUND		0
#define BACKGROUND		1

typedef int pid_t;

typedef int tty_t;

typedef void (pfunc_t)(int);

/* System Calls */

/**
 *	@fn pid_t createProcess(char *name, pfunc_t main, void *args)
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
pid_t createProcess(char *name, pfunc_t main, int args, int level);

/**
 *	@fn void exit(int status)
 *
 * 		@brief This function terminates the calling process
 * 		and status is returned to whom is waiting for the
 * 		calling process to exit.
 *		@param status The process' exit status.
 *
 */
void exit(int status);

/**
 *	@fn int kill(int pid, int signal)
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
int kill(int pid, int signal);

/**
 *	@fn pid_t wait(int *status)
 *
 *		@brief This function is used to wait for
 *		state changes in a child of the calling process and
 *		obtain information about the child whose state changed.
 *		@param status A pointer to where the child process status
 *		will be placed.
 *		@return The child process' pid.
 *
 */
pid_t wait(int *status);

/*
 *	@fn unsigned int sleep(unsigned int seconds);
 *
 *		@brief This function makes the calling process
 *		sleep until 'seconds' have elapsed or a signal
 *		arrives which is not ignored.
 *		@param seconds The number of seconds the calling process will sleep.
 *		@return Zero if the requested time has elapsed or the remaining seconds
 *		if the sleep was interrupted by a signal.
 */
unsigned int sleep(unsigned int seconds);

/*
 *	@fn pid_t getpid(void)
 *
 *		@brief This function gets the pid of the calling process.
 */
pid_t getpid(void);

/*
 * @fn pid_t getppid(void)
 *
 * 		@brief This function gets the pid of the calling process' parent.
 */
pid_t getppid(void);

/*
 * @fn pid_t getppid(void)
 *
 * 		@brief This function gets the gid of the calling process.
 */
pid_t getpgrp(void);

/*
 *	@fn int setpgid(pid_t pid, pid_t pgid)
 *
 *		@brief This function sets the gid of the process identified by 'pid'
 *		with 'pgid'. If 'pid' is set to 0 the calling process' id is used.
 *		If 'pgid' is set to 0 the calling process' gid is used.
 *		@param pid The process' pid.
 *		@param pgid The process' gid.
 *		@return 0 on success or -1 in case of error.
 */
int setpgid(pid_t, pid_t pgid);

int dup(int fd1);

int dup2(int fd1, int fd2);

int setlevel(int level);

int getlevel(void);

int setpriority(pid_t pid, int prio);

int getpriority(void);

unsigned time(void);

tty_t getTty(pid_t pid);

tty_t setTty(pid_t pid, tty_t tty);

#endif
