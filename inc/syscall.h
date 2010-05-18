/**
 *	\file syscall.h
 *
 * 		\brief Implementation of every system call.
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

#endif
