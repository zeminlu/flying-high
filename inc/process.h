#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "syscall.h"
#include "int80.h"
#include "types.h"
#include "string.h"

#define FOREGROUND		0
#define BACKGROUND		1


void * memmap();

pid_t getpid();

pid_t getppid();

tty_t getCurrentTTY();

int getProccessByPid( pid_t pid );

int getProccessByName( char * name );

#endif
