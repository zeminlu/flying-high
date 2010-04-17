#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "syscall.h"
#include "int80.h"
#include "types.h"

#define FOREGROUND		0
#define BACKGROUND		1

typedef int pid_t;

typedef int tty_t;

typedef void (pfunc_t)(int);

void * memmap();

#endif