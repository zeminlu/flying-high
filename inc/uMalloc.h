/**
 *	\file uMalloc.h
 *
 *		\brief User malloc module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _U_MALLOC_H_
#define	_U_MALLOC_H_

#include "memModule.h"
#include "sysMalloc.h"
#include "process.h"

/**
 * \fn void free(void *ap)
 *
 * 		\brief lets avaiable the user memory for futures mallocs.
 *
 * 		\param ap the logical memory position that would be freed.
 * 		
 * 		\sa malloc() realloc()
 *
 */
void free(void *ap);

/**
 * \fn void *malloc(size_t nbytes)
 *
 * 		\brief	returns a continues segment of user memory. It could return null if there is enought free space.
 *
 * 		\param nbytes The size of continues memory that wolud be returned.
 * 		
 * 		\return The logical user memory to be used.
 *
 * 		\sa free() realloc()
 *
 */
void *malloc(size_t nbytes);

/**
 * \fn void *realloc(void *ap, size_t size)
 *
 * 		\brief Change the original size of the continues memory. If is posible moves the old segment
 *   			user memory , into the a new memory continious user memory segment .
 *
 * 		\param ap The old position that would be change if it necesari.
 * 		\param size The new size of the continues user memory.
 * 		
 * 		\return a new  continues segment of user memory.
 *
 * 		\sa free malloc()
 *
 */
void *realloc(void *ap, size_t size);

#endif