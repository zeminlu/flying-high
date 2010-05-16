/**
 *	\file sysProcess.h
 *
 *		\brief Brief.
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
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void forceMultitasker();

/**
 * \fn int isMTActivated()
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
int isMTActivated();

/**
 * \fn int initMultitasker(pfunc_t init)
 *
 * 		\brief Brief.
 *
 * 		\param init ParamBrief.
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
int initMultitasker(pfunc_t init);

/**
 * \fn void sysSelfBlock()
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void sysSelfBlock();

/**
 * \fn int sysUnblock(pid_t pid)
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
int sysUnblock(pid_t pid);

/**
 * \fn void waitTty(tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void waitTty(tty_t tty);

/**
 * \fn void signalTty(tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void signalTty(tty_t tty);

/**
 * \fn void terminate(pid_t pid, int status)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param status ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void terminate(pid_t pid, int status);

/**
 * \fn process_t *getProcessTable(void)
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
process_t *getProcessTable(void);

/**
 * \fn char *getProcessName(pid_t pid)
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
char *getProcessName(pid_t pid);

/**
 * \fn void refreshProcessesCPUs()
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void refreshProcessesCPUs();

/**
 * \fn void clearProcessesTicks()
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void clearProcessesTicks();

#endif