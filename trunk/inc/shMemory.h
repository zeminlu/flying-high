/**
 *	\file shMemory.h
 *
 *		\brief Brief.
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
 * 		\brief Brief.
 *
 * 		\param key ParamBrief.
 * 		\param size ParamBrief.
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
int shmget(key_t key, int size);

/**
 * \fn char * shmat(int shmid, key_t *semid)
 *
 * 		\brief Brief.
 *
 * 		\param shmid ParamBrief.
 * 		\param semid ParamBrief.
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
char * shmat(int shmid, key_t *semid);

/**
 * \fn int shmdt(int shmid)
 *
 * 		\brief Brief.
 *
 * 		\param shmid ParamBrief.
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
int shmdt(int shmid);

/**
 * \fn void initializeShMems()
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
void initializeShMems();

#endif