/**
 *	\file syscall.h
 *
 * 		\brief Definitions for all system call constants.
 *
 * 		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "types.h"
#include "memModule.h"
#include "process.h"
#include "file.h"
#include "string.h"
#include "kasm.h"
#include "schedule.h"
#include "kernelDepth.h"
#include "ttys.h"

#define EFLAGS					0x00200
#define STACK_TRACE_LENGTH 		15
#define KILL_EXIT_STATUS 		0
#define LOCAL_VARIABLES_SPACE	0x00000018
#define MILISECONDS_PER_TICK	55

enum{
	_SYS_WRITE,
	_SYS_READ,
	_SYS_MEMMAP,
	_SYS_CREATE,
	_SYS_EXIT,
	_SYS_SET_TTY,	
	_SYS_GET_TTY,
	_SYS_GET_PID,
	_SYS_GET_PPID,
	_SYS_WAIT_PID,
	_SYS_KILL
};

pid_t _sys_create_process(char *name, pfunc_t main, int args, int level);

void _sysExit(int status);

void * _sys_memmap(int isKernel);

tty_t _sys_get_tty(pid_t pid);

tty_t _sys_set_tty(pid_t pid, tty_t tty);

pid_t _sys_get_pid();

pid_t _sys_get_ppid();

int _sys_wait_pid(pid_t pid, int *status, int opt);

int _sys_kill(int fd1, int fd2);

void cli(void);

void sti(void);

#endif
