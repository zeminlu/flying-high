#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "syscall.h"
#include "int80.h"
#include "types.h"
#include "string.h"
#include "sysProcess.h"

#define FOREGROUND		0
#define BACKGROUND		1

void * memmap(int isKernel);

pid_t getpid();

pid_t getppid();

tty_t getCurrentTTY();
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
int getProccessByPid( pid_t pid );

int getProccessByName( char * name );

#endif
