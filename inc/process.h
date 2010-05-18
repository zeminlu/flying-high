/**
 *	\file process.h
 *
 *		\brief All the process functions that invoke syscalls.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "syscall.h"
#include "int80.h"
#include "types.h"
#include "string.h"
#include "sysProcess.h"

/**
 * \fn void * memmap(int isKernel)
 *
 * 		\brief Brief.
 *
 * 		\param isKernel ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void * memmap(int isKernel);

/**
 * \fn pid_t getpid()
 *
 * 		\brief Gets a pointer to the current process heap, or kernel heap.
 *
 * 		\param isKernel TRUE if it's kernel, FALSE if normal process.
 * 		
 * 		\return A pointer to the heap space.
 *
 */
pid_t getpid();

/**
 * \fn pid_t getppid()
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
pid_t getppid();

/**
 *	\fn pid_t createProcess(char *name, pfunc_t main, void *args, int level)
 *		
 *		\brief Creates a new process into the system and sets
 *		it ready to run. The process will be created in unatomic
 *		mode(preemptive mode) and with tty -1.
 *		
 *		\param name The process' name
 *		\param main The process' main function. Must Not Be NULL.
 *		\param args A pointer to an argument list.
 *		\param level FOREGROUND or BACKGROUND.
 *
 *		\return The new process' pid or -1 in case of error.
 *
 */
pid_t createProcess(char *name, pfunc_t main, void *args, int level);

/**
 *	\fn void exit(int status)
 *
 * 		\brief This function terminates the calling process
 * 				and status is returned to whom is waiting for the
 * 				calling process to exit.
 *		\param status The process' exit status.
 *
 */
void exit(int status);

/**
 * \fn tty_t setTty(pid_t pid, tty_t tty)
 *
 * 		\brief Sets the given process tty id.
 *
 * 		\param pid The process' pid who's tty id will be set.
 * 		\param tty The tty id to be set.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
tty_t setTty(pid_t pid, tty_t tty);

/**
 * \fn tty_t getTty(pid_t pid)
 *
 * 		\brief Retrieves the given process pid's tty.
 *
 * 		\param pid The pid of the process.
 * 		
 * 		\return The tty id.
 *
 * 		\sa setTty()
 *
 */
tty_t getTty(pid_t pid);

/**
 * \fn tty_t setTTYFocusedProcess(pid_t pid, tty_t tty)
 *
 * 		\brief Sets the given tty focused process with the given pid process.
 *
 * 		\param pid The process to be set as focused process.
 * 		\param tty The tty who's focused process will be set.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		\sa getTTYFocusedProcess()
 *
 */
tty_t setTTYFocusedProcess(pid_t pid, tty_t tty);

/**
 * \fn pid_t getTTYFocusedProcess(tty_t tty)
 *
 * 		\brief Retrieves the given tty focused process pid.
 *
 * 		\param tty The tty id.
 * 		
 * 		\return The pid of the tty focused process.
 *
 * 		\sa setTTYFocusedProcess()
 *
 */
pid_t getTTYFocusedProcess(tty_t tty);

/**
 * \fn pid_t getpid(void)
 *
 * 		\brief Retrieves the calling process' pid.
 *
 * 		\return The calling process' pid.
 *
 */
pid_t getpid(void);

/**
 * \fn pid_t getppid()
 *
 * 		\brief Retrieves the calling process' parent pid.
 *
 * 		\return The calling process' parent pid.
 *
 */
pid_t getppid();

/**
 * \fn int wait(int *status)
 *
 * 		\brief Blocks the calling process until a child returns.
 *
 * 		\param status A pointer where to store the return state of the child process.
 * 		
 * 		\return Returns the pid of the child process that recently returned.
 *
 * 		\sa waitpid()
 *
 */
int wait(int *status);

/**
 * \fn int waitpid(pid_t pid, int *status)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param status ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int waitpid(pid_t pid, int *status);

/**
 * \fn int kill(int pid)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int kill(int pid);

/**
 * \fn int setTTYMode(pid_t pid, int mode)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param mode ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int setTTYMode(pid_t pid, int mode);

/**
 * \fn int getTTYMode(pid_t pid)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int getTTYMode(pid_t pid);

/**
 * \fn int setLevel(int level)
 *
 * 		\brief Brief.
 *
 * 		\param level ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int setLevel(int level);

/**
 * \fn int getLevel()
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int getLevel();

/**
 * \fn int getPriority()
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int getPriority();

/**
 * \fn int setPriority(pid_t pid, int prio)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param prio ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int setPriority(pid_t pid, int prio);

/**
 * \fn unsigned time(void)
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
unsigned time(void);

/**
 * \fn unsigned upTime()
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
unsigned upTime();

/**
 * \fn int setProcessAtomicity(pid_t pid, int atomic)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param atomic ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int setProcessAtomicity(pid_t pid, int atomic);

#endif
