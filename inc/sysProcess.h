/**
 *	\file sysProcess.h
 *
 *		\brief OS system utilities.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _SYSPROCESS_H_
#define _SYSPROCESS_H_

#include "ttys.h"
#include "keyboard_buffer.h"
#include "types.h"
#include "memModule.h"	
#include "schedule.h"
#include "syscall.h"
#include "stdio.h"
#include "kMalloc.h"
#include "rand.h"
#include "shell.h"

/**
 * \fn void forceMultitasker()
 *
 * 		\brief Forces the call to the multitasker.
 *
 */
void forceMultitasker();

/**
 * \fn int isMTActivated()
 *
 * 		\brief Checks wether the multitasker is activated or not.
 *
 * 		\return True if it's activated, False if not.
 *
 */
int isMTActivated();

/**
 * \fn int initMultitasker(pfunc_t init)
 *
 * 		\brief Initializes the multitasker and creates the init process.
 *
 * 		\param init A pointer to the init function.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		\sa multitasker()
 *
 */
int initMultitasker(pfunc_t init);

/**
 * \fn void sysSelfBlock()
 *
 * 		\brief Blocks the calling process.
 *
 * 		\sa sysUnblock()
 *
 */
void sysSelfBlock();

/**
 * \fn int sysUnblock(pid_t pid)
 *
 * 		\brief Unblocks the given pid process.
 *
 * 		\param pid The pid of the process to be unlocked.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		\sa sysSelfBlock()
 *
 */
int sysUnblock(pid_t pid);

/**
 * \fn void waitTty(tty_t tty)
 *
 * 		\brief Blocks the calling process until it's tty is signaled.
 *
 * 		\param tty The tty of the process to be blocked.
 * 		
 * 		\sa signalTty()
 *
 */
void waitTty(tty_t tty);

/**
 * \fn void signalTty(tty_t tty)
 *
 * 		\brief Signals the given tty so that any blocked process from it can be unblocked.
 *
 * 		\param tty The tty to be signaled.
 * 		
 * 		\sa waitTty()
 *
 */
void signalTty(tty_t tty);

/**
 * \fn void terminate(pid_t pid, int status)
 *
 * 		\brief Terminates the given process with the given status.
 *
 * 		\param pid The pid number from the process to be terminated.
 * 		\param status The status upon which the process is being terminated.
 * 		
 */
void terminate(pid_t pid, int status);

/**
 * \fn process_t *getProcessTable(void)
 *
 * 		\brief Retrieves the OS process table.
 *
 * 		\return A pointer to the OS process table.
 *
 */
process_t *getProcessTable(void);

/**
 * \fn char *getProcessName(pid_t pid)
 *
 * 		\brief Retrieves the given pid process name.
 *
 * 		\param pid The pid from the process whose name to be retrieven.
 * 		
 * 		\return The process' name.
 *
 */
char *getProcessName(pid_t pid);

/**
 * \fn void refreshProcessesCPUs()
 *
 * 		\brief Refreshes the process comsumption of every process.
 *
 * 		\sa clearProcessesTicks()
 *
 */
void refreshProcessesCPUs();

/**
 * \fn void clearProcessesTicks()
 *
 * 		\brief Clears the current tickcount of every process.
 *
 * 		\sa refreshProcesses()
 *
 */
void clearProcessesTicks();

#endif