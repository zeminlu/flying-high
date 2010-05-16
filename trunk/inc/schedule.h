/**
 *	\file schedule.h
 *
 *		\brief Brief.
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
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
process_t * rpgSchedule();

/**
 * \fn process_t * getNextTask( void )
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
process_t * getNextTask( void );

/**
 * \fn int checkWhatAreReady( void )
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int checkWhatAreReady( void );

/**
 * \fn int getTheOlder( process_t * pro[] )
 *
 * 		\brief Brief.
 *
 * 		\param pro ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int getTheOlder( process_t * pro[] );

/**
 * \fn void increaseRPGCounter( void )
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void increaseRPGCounter( void );

/**
 * \fn void setupScheduler( void )
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void setupScheduler( void );

/**
 * \fn void changeAlgorithimSchedule( void )
 *
 * 		\brief Brief.
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