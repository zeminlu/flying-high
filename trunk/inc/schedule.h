/*
 *	schedule.h
 */

#ifndef SCEHDULE
#define SCEHDULE

#define	ROUND_ROBIN	1
#define	RPG			2
#define	ALGORITHIM	ROUND_ROBIN
#define	MAX_RPG		100

#define	evaluate(x)		((x * 0.75) + (MAX_PROCESS/(x+1)) * 3)
#define	MAX(a,b)		((a > b) ? a : b)

#include "sysProcess.h"

process_t * getNextTask();

process_t * roundRobinSchedule();

process_t * rpgSchedule();

void checkWhatAreReady( process_t * pro[] );

int getTheOlder( process_t * pro[] );

void increaseRPGCounter();

#endif