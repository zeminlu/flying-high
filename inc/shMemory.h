/**
 *	\file shMemory.h
 *
 *		\brief The shared memory IPC module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#ifndef _SHMEMORY_H_
#define _SHMEMORY_H_

#include "semaphore.h"

#define	IPC_RMID	1
#define IPC_SET		2
#define IPC_STAT	3
#define IPC_CREAT	4
#define	IPC_PRIVATE 5

#define MAX_SHMEMS	8
#define MAX_SHMAPPN	MAX_PROCESS

/**
 * \fn int shmget(key_t key, int size)
 *
 * 		\brief Gets a new shared memory with the given key and size.
 *
 * 		\param key The key for the shared memory segment.
 * 		\param size The shared memory size.
 * 		
 * 		\return The id of the shared memory on success, -1 on error.
 *
 * 		\sa shmat() shmdt()
 *
 */
int shmget(key_t key, int size);

/**
 * \fn char * shmat(int shmid, key_t *semid)
 *
 * 		\brief Attaches the current process to the shmid shared memory.
 *
 * 		\param shmid The id of the shared memory segment.
 * 		\param semid A pointer to a key_t variable where to store the shared memory inner semaphore id.
 * 		
 * 		\return A pointer to the shared memory space.
 *
 * 		\sa shmget() shmdt()
 *
 */
char * shmat(int shmid, key_t *semid);

/**
 * \fn int shmdt(int shmid)
 *
 * 		\brief Detaches the current process form the shmid shared memory segment.
 *
 * 		\param shmid The id of the shared memory segment.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		\sa shmget() shmat()
 *
 */
int shmdt(int shmid);

/**
 * \fn void initializeShMems()
 *
 * 		\brief Initializes all the shared memories.
 *
 */
void initializeShMems();

#endif