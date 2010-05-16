/**
 *	\file semaphore.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "types.h"
#include "process.h"

#define BLOCK 0
#define WAIT 1
#define MAX_SEMS 64
#define UNUSED 0
#define USED !UNUSED

/**
 * \fn key_t sem_get(int mode)
 *
 * 		\brief Brief.
 *
 * 		\param mode ParamBrief.
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
key_t sem_get(int mode);

/**
 * \fn void sem_free( key_t semid )
 *
 * 		\brief Brief.
 *
 * 		\param semid ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void sem_free( key_t semid );

/**
 * \fn void sem_wait( key_t sem )
 *
 * 		\brief Brief.
 *
 * 		\param sem ParamBrief.
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
void sem_wait( key_t sem );

/**
 * \fn void sem_signal( key_t sem )
 *
 * 		\brief Brief.
 *
 * 		\param sem ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void sem_signal( key_t sem );

/**
 * \fn void initializeSemaphores()
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
void initializeSemaphores();

#endif