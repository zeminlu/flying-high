/**
 *	\file kMalloc.h
 *
 *		\brief The kernel malloc module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef K_MALLOC_H_
#define	K_MALLOC_H_

#include "memModule.h"
#include "sysMalloc.h"
#include "process.h"
#include "stdio.h"

/**
 * \fn void kFree(void *ap)
 *
 * 		\brief 	This function calls sysFree, that leaves the kernel memory to be used by Kalloc in future calls.
 *
 * 		\param 	ap The kernel memory position that will be freed. 
 * 		
 * 		\sa kMalloc() kRealloc()
 *
 */
void kFree(void *ap);

/**
 * \fn void *kMalloc(size_t nbytes)
 *
 * 		\brief 	This funtion calls sysMalloc that gives the request size of memory. 
 *				This is given from the kernel memory. On error returns null.
 *
 * 		\param	nbytes The memory size that would be returned, this is allways kernel memory.
 * 		
 * 		\return	Returns the logic memory position, with continious memory to use.
 *
 * 		\sa kFree() kRealloc()
 *
 */
void *kMalloc(size_t nbytes);

/**
 * \fn void *kRealloc(void *ap, size_t size)
 *
 * 		\brief	Calls sysRealloc, that changes the size of the original. It returns null if 
 *				there is not enough memory.
 *
 * 		\param ap The logic memory position that wold be extended.
 * 		\param size	The new size that would be used to increase the ap segment memory.
 * 		
 * 		\return	The new memory address that has a continuous segment of kernel memory.
 *
 * 		\sa kFree() kMalloc()
 *
 */
void *kRealloc(void *ap, size_t size);

#endif