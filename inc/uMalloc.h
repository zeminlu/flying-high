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
 * 		\brief Frees up the user memory pointed by ap for futures mallocs.
 *
 * 		\param ap Pointer to the logical memory position that would be freed.
 * 		
 * 		\sa malloc() realloc()
 *
 */
void free(void *ap);

/**
 * \fn void *malloc(size_t nbytes)
 *
 * 		\brief	returns a continuous segment of user memory. It could return null if there is not enough free space.
 *
 * 		\param nbytes The size of continuous memory that would be returned.
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
 * 		\brief Changes the original size of the continuous memory. If it is possible, it moves the old segment
 *   			of user memory to a new continuous user memory segment .
 *
 * 		\param ap The old position that would be changed if it's necessary.
 * 		\param size The new size of the continuous user memory.
 * 		
 * 		\return A new continuous segment of user memory.
 *
 * 		\sa free() malloc()
 *
 */
void *realloc(void *ap, size_t size);

#endif