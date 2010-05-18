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
 * 		\brief 	This function simulate the RPG scheduling. The algorithim is about
 *				that when a process achieve the maximum value of RPG, it is adding to
 *				a queue. After that the functions just founf the oldest process who are
 *				waiting to be attending.
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
 * 		\brief  This function return the next task that should be
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
 * 		\brief This function check what process are ready using RPG
 *				Algorithim
 *
 * 		\return the index of table process
 *
 */
int checkWhatAreReady( void );

/**
 * \fn void increaseRPGCounter( void )
 *
 * 		\brief This function increase the current rpg value
 *				of each process.
 *
 */

void increaseRPGCounter( void );

/**
 * \fn void changeAlgorithimSchedule( void )
 *
 * 		\brief 	This function change the Algorithim
 *				variable, if the current algorithim is RoundRobin
 *				it changes to RPG and viceversa.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void changeAlgorithimSchedule( void );

#endif