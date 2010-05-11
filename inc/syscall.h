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

size_t _sys_write ( int fd, char * buffer, size_t n );

size_t _sys_read ( int fd, char * buffer, size_t n );

pid_t _sys_create_process(char *name, pfunc_t main, int args, int level);

void _sysExit(int status);

void * _sys_memmap(int isKernel);

tty_t _sys_get_tty(pid_t pid);

tty_t _sys_set_tty(pid_t pid, tty_t tty);

pid_t _sys_get_pid();

pid_t _sys_get_ppid();

pid_t _sys_wait(int *status);

int _sys_wait_pid(pid_t pid, int *status);

int _sys_kill(int pid);

int _sys_set_level(int level);

int _sys_get_level();

int _sys_get_priority();

int _sys_set_priority(pid_t pid, int prio );

unsigned _sys_time(void);

int _sys_set_atomicity(pid_t pid, int atomic);

#endif
