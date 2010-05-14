/*
 *	rand.h
 */

#ifndef RAND
#define RAND

#include "syscall.h"

#define RAND_MAX 42949679

unsigned int timeRand();

int rand();

void srand( unsigned int seedP );

#endif