/**
 *	\file syscall.h
 *
 * 		\brief Definitions for all system call constants.
 *
 * 		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "types.h"
#include "memModule.h"
#include "process.h"
#include "string.h"
#include "kasm.h"
#include "schedule.h"
#include "kernelDepth.h"
#include "ttys.h"

#define EFLAGS					0x00200
#define STACK_TRACE_LENGTH 		15
#define KILL_EXIT_STATUS 		0
#define LOCAL_VARIABLES_SPACE	0x00000018

/**
 * \fn size_t _sys_write ( int fd, char * buffer, size_t n )
 *
 * 		\brief Brief.
 *
 * 		\param fd ParamBrief.
 * 		\param buffer ParamBrief.
 * 		\param n ParamBrief.
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
size_t _sys_write ( int fd, char * buffer, size_t n );

/**
 * \fn size_t _sys_read ( int fd, char * buffer, size_t n )
 *
 * 		\brief Brief.
 *
 * 		\param fd ParamBrief.
 * 		\param buffer ParamBrief.
 * 		\param n ParamBrief.
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
size_t _sys_read ( int fd, char * buffer, size_t n );

/**
 * \fn pid_t _sys_create_process(char *name, pfunc_t main, int args, int level)
 *
 * 		\brief Brief.
 *
 * 		\param name ParamBrief.
 * 		\param args ParamBrief.
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
pid_t _sys_create_process(char *name, pfunc_t main, int args, int level);

/**
 * \fn void _sysExit(int status)
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
void _sysExit(int status);

/**
 * \fn void * _sys_memmap(int isKernel)
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
void * _sys_memmap(int isKernel);

/**
 * \fn tty_t _sys_get_tty(pid_t pid)
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
tty_t _sys_get_tty(pid_t pid);

/**
 * \fn tty_t _sys_set_tty(pid_t pid, tty_t tty)
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
tty_t _sys_set_tty(pid_t pid, tty_t tty);

/**
 * \fn pid_t _sys_get_pid()
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
pid_t _sys_get_pid();

/**
 * \fn pid_t _sys_get_ppid()
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
pid_t _sys_get_ppid();

/**
 * \fn pid_t _sys_wait(int *status)
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
pid_t _sys_wait(int *status);

/**
 * \fn int _sys_wait_pid(pid_t pid, int *status)
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
int _sys_wait_pid(pid_t pid, int *status);

/**
 * \fn int _sys_kill(int pid)
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
int _sys_kill(int pid);

/**
 * \fn int _sys_set_tty_mode(pid_t pid, int mode)
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
int _sys_set_tty_mode(pid_t pid, int mode);

/**
 * \fn int _sys_get_tty_mode(pid_t pid)
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
int _sys_get_tty_mode(pid_t pid);

/**
 * \fn int _sys_set_level(int level)
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
int _sys_set_level(int level);

/**
 * \fn int _sys_get_level()
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
int _sys_get_level();

/**
 * \fn int _sys_get_priority()
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
int _sys_get_priority();

/**
 * \fn int _sys_set_priority(pid_t pid, int prio )
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
int _sys_set_priority(pid_t pid, int prio );

/**
 * \fn unsigned _sys_time(void)
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
unsigned _sys_time(void);

/**
 * \fn unsigned _sys_uptime()
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
unsigned _sys_uptime();

/**
 * \fn int _sys_set_atomicity(pid_t pid, int atomic)
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
int _sys_set_atomicity(pid_t pid, int atomic);

#endif
