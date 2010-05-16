/**
 *	\file process.h
 *
 *		\brief Brief.
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

tty_t getCurrentTTY();
/**
 *	\fn pid_t createProcess(char *name, pfunc_t main, void *args)
 *		
 *		\brief Creates a new process into the system and sets
 *		it ready to run. The process will be created in unatomic
 *		mode(preemptive mode) and with tty -1.
 *		\param name The process' name
 *		\param main The process' main function. Must Not Be NULL.
 *		\param args A pointer to an argument list.
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
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param tty ParamBrief.
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
tty_t setTty(pid_t pid, tty_t tty);

/**
 * \fn tty_t getTty(pid_t pid)
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
tty_t getTty(pid_t pid);

/**
 * \fn tty_t setTTYFocusedProcess(pid_t pid, tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param tty ParamBrief.
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
tty_t setTTYFocusedProcess(pid_t pid, tty_t tty);

/**
 * \fn tty_t getTTYFocusedProcess(tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
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
tty_t getTTYFocusedProcess(tty_t tty);

/**
 * \fn pid_t getpid(void)
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
pid_t getpid(void);

/**
 * \fn pid_t getppid()
 *
 * 		\brief Brief.
 *
 * 		\param parameter ParamBrief.
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
 * \fn int wait(int *status)
 *
 * 		\brief Brief.
 *
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
