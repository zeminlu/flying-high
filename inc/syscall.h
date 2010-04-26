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

#define _SYS_WRITE 	0
#define _SYS_READ 	1
#define _SYS_MEMMAP 2
#define _SYS_CREATE 3
#define _SYS_EXIT 4

#define EFLAGS			0x00200
#define STACK_TRACE_LENGTH 15
#define KILL_EXIT_STATUS 0
#define LOCAL_VARIABLES_SPACE	0x0000001c
#define MILISECONDS_PER_TICK	55

pid_t _sys_create_process(char *name, pfunc_t main, int args, int level);

void _sysExit(int status);

void * _sys_memmap(int isKernel);

#endif
