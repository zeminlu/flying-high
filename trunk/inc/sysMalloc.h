/**
 *	\file sysMalloc.h
 *
 *		\brief Generic malloc module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef SYSMALLOC_H_
#define	SYSMALLOC_H_

#include "memModule.h"
#include "string.h"

/**
 * \fn void sysFree(void *ap, memArea_t * (*getMemoryArea)())
 *
 * 		\brief Leaves avaiable the segment to be used in futures calls to sysMalloc. The memory
 *  			segment is taken from the function pased by arguments.
 *
 * 		\param ap The logical memory segment to be freed.
 * 		\param getMemoryArea Function that returns the memory frame that sysMalloc wold be used in the future.
 * 		
 * 		\sa sysRealloc() sysMalloc()
 *
 */
void sysFree(void *ap, memArea_t * (*getMemoryArea)());

/**
 * \fn void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)())
 *
 * 		\brief Function that gives a continuous segment of memory. This memory comes from the
 * 				function getMemoryArea. It could return null if there is not enougth free space
 *				on that frame.
 *
 * 		\param nbytes the size of continuous segment memory.
 * 		\param getMemoryArea Functions that gives the frame to use be used by sysMalloc.
 * 		
 * 		\return The continuous segment of memory.
 *
 * 		\sa sysFree() sysRealloc()
 *
 */
void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)());

/**
 * \fn void *sysRealloc(void *ptr, size_t size, memArea_t * (*getMemoryArea)())
 *
 * 		\brief This function changes the original size of the logical memory to a new one if
 *  				it's necesary.Returns null if theres not enough free space.
 *
 * 		\param ptr Old logical memory segment, that could be moved to a new one.
 * 		\param size The new size of the continuous memory segment.
 * 		\param getMemoryArea Function that gives the frame to use be used by sysMalloc.
 * 		
 * 		\return The new continuous memory segment.
 *
 * 		\sa sysFree() sysMalloc()
 *
 */
void *sysRealloc(void *ap, size_t size, memArea_t * (*getMemoryArea)());

#endif