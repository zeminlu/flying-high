/*
 *	schedule.h
 */

#ifndef SCEHDULE
#define SCEHDULE

#include "sysProcess.h"
#include "uMalloc.h"

#define	ROUND_ROBIN	1
#define	RPG			2
#define	ALGORITHIM	ROUND_ROBIN
#define	MAX_RPG		100

#define	evaluate(x)		((x * 0.75) + ((x+2)/MAX_PROCESS) * 100)
#define	MAX(a,b)		((a > b) ? a : b)


static process_t * getNextTask( void );

static process_t * roundRobinSchedule( void );

static process_t * rpgSchedule( void );

void checkWhatAreReady( process_t * pro[] );

int getTheOlder( process_t * pro[] );

void increaseRPGCounter( void );

void setupScheduler( void );

#endif