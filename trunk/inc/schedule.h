/*
 *	schedule.h
 */

#ifndef SCEHDULE
#define SCEHDULE

#include "types.h"
#include "uMalloc.h"

#define	ROUND_ROBIN	1
#define	RPG			2
#define	MAX_RPG		100

#define	evaluate(x)		((x * 0.75) + ((x+2)/MAX_PROCESS) * 100)
#define	MAX(a,b)		((a > b) ? a : b)

process_t * rpgSchedule();

process_t * getNextTask( void );

void checkWhatAreReady( process_t * pro[] );

int getTheOlder( process_t * pro[] );

void increaseRPGCounter( void );

void setupScheduler( void );

void changeAlgorithimSchedule( void );

#endif