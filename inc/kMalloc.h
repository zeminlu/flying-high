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
 *				This is given from the kernle memory. On error return null.
 *
 * 		\param	nbytes The memory size that would be returned, this is allways kernel memory,.
 * 		
 * 		\return	Return the memory logic position , with continious memory to use.
 *
 * 		\sa kFree() kRealloc()
 *
 */
void *kMalloc(size_t nbytes);

/**
 * \fn void *kRealloc(void *ap, size_t size)
 *
 * 		\brief	Calls sysRealloc, that change the size of the original. It returns null if 
 *				there is not enougth memory.
 *
 * 		\param ap The logic memory position that wold be extended.
 * 		\param size	The new size that would be used to increase the ap segment memory.
 * 		
 * 		\return	the new memory address that have a continious segment of kernel memory.
 *
 * 		\sa kFree() kMalloc()
 *
 */
void *kRealloc(void *ap, size_t size);

#endif