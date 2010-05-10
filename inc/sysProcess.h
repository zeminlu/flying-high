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

void forceMultitasker();

void idle();

void chupala();

void puto();

void de();

void mierda();

void anda();

void todo();

void carajo();

int isMTActivated();

int initMultitasker(pfunc_t init);

void terminate(pid_t pid, int status);

void sysSelfBlock();

int sysUnblock(pid_t pid);

pid_t sysWait(int *status);

pid_t sysWaitpid(pid_t pid, int *status, int options);

void waitTty(tty_t tty);

void signalTty(tty_t tty);

tty_t sysGetTty(pid_t pid);

tty_t sysSetTty(pid_t pid, tty_t tty);

void initializeFileSystem( void );


#endif