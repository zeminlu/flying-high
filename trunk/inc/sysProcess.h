#ifndef _SYSPROCESS_H_
#define _SYSPROCESS_H_

#include "ttys.h"
#include "keyboard_buffer.h"
#include "types.h"
#include "memModule.h"	
#include "schedule.h"
#include "syscall.h"
#include "stdio.h"

void forceMultitasker();

void chupala();

void puto();

int isMTActivated();

int initMultitasker(pfunc_t init);

void terminate(pid_t pid, int status);

pid_t sysWait(int *status);

pid_t sysWaitpid(pid_t pid, int *status, int options);

void waitTty(tty_t tty);

void signalTty(tty_t tty);

tty_t sysGetTty(pid_t pid);

tty_t sysSetTty(pid_t pid, tty_t tty);

#endif