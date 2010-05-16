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

void forceMultitasker();

int isMTActivated();

int initMultitasker(pfunc_t init);

void sysSelfBlock();

int sysUnblock(pid_t pid);

void waitTty(tty_t tty);

void signalTty(tty_t tty);

void terminate(pid_t pid, int status);

process_t *getProcessTable(void);

char *getProcessName(pid_t pid);

void refreshProcessesCPUs();

void clearProcessesTicks();

#endif