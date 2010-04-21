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

#include "sysProcess.h"
#include "memModule.h"
#include "process.h"
#include "file.h"
#include "string.h"
#include "kasm.h"

#define _SYS_WRITE 	0
#define _SYS_READ 	1
#define _SYS_MEMMAP 2
#define _SYS_CREATE_PROCESS 3
#define _SYS_EXIT 4

#define EFLAGS			0x00200
#define STACK_TRACE_LENGTH 15
#define KILL_EXIT_STATUS 0
#define LOCAL_VARIABLES_SPACE	0x0000001c
#define MILISECONDS_PER_TICK	55

#endif
