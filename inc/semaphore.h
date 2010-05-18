/**
 *	\file semaphore.h
 *
 *		\brief The OS semaphore module.
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
 * 		\brief Gets a semaphore for the given mode.
 *
 * 		\param mode Block or Wait modes.
 * 		
 * 		\return The key of the semaphore.
 *
 * 		\sa sem_free()
 *
 */
key_t sem_get(int mode);

/**
 * \fn void sem_free( key_t semid )
 *
 * 		\brief Frees up the given semid semaphore.
 *
 * 		\param semid The id of the semaphore.
 * 		
 * 		\sa sem_get()
 *
 */
void sem_free( key_t semid );

/**
 * \fn void sem_wait( key_t sem )
 *
 * 		\brief Waits for the semaphore to be available..
 *
 * 		\param sem The semaphore id.
 * 		
 * 		\sa sem_signal()
 *
 */
void sem_wait( key_t sem );

/**
 * \fn void sem_signal( key_t sem )
 *
 * 		\brief Signals the given semaphore to enable it for other users.
 *
 * 		\param sem The semaphore id.
 * 		
 * 		\sa sem_free()
 *
 */
void sem_signal( key_t sem );

/**
 * \fn void initializeSemaphores()
 *
 * 		\brief Initializes all semaphores.
 *
 */
void initializeSemaphores();

#endif