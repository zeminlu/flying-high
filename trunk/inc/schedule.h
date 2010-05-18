/**
 *	\file schedule.h
 *
 *		\brief The OS scheduler module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#ifndef SCEHDULE
#define SCEHDULE

#include "types.h"
#include "uMalloc.h"
#include "video_driver.h"

#define	ROUND_ROBIN	1
#define	RPG			2
#define	MAX_RPG		10

#define	evaluate(x)		((x * 0.75) + ((x+2)/MAX_PROCESS) * 100)
#define	MAX(a,b)		((a > b) ? a : b)

/**
 * \fn process_t * rpgSchedule()
 *
 * 		\brief 	This function simulates the RPG scheduling. What this algorithm does, is that
 * 				when a process achieves the maximum value of RPG, it adds it to
 *				a queue. After that the functions just find the oldest process that is
 *				waiting to be attended.
 *
 * 		\return The process to be attended
 *
 * 		Use:
 * 		\code
 *			if( ALGORITHIM == RPG )
 *				return rpgSchedule();
 *		\endcode
 *
 */
process_t * rpgSchedule();

/**
 * \fn process_t * getNextTask( void )
 *
 * 		\brief  This function returns the next task that should be
 *				attended.
 *
 * 		\return The next process
 *
 * 		Use:
 * 		\code
 *			changeContext(getNextTask());
 *		\endcode
 *
 */
process_t * getNextTask( void );

/**
 * \fn int checkWhatAreReady( void )
 *
 * 		\brief This function checks what processes are ready using the RPG Algorithim
 *
 * 		\return the index of table process
 *
 */
int checkWhatAreReady( void );

/**
 * \fn void increaseRPGCounter( void )
 *
 * 		\brief This function increases the current rpg value
 *				of each process.
 *
 */

void increaseRPGCounter( void );

/**
 * \fn void changeAlgorithimSchedule( void )
 *
 * 		\brief 	This function changes the Algorithm
 *				variable, if the current algorithm is RoundRobin
 *				it changes to RPG and viceversa.
 *
 */
void changeAlgorithimSchedule( void );

#endif