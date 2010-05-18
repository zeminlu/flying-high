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
 * 		\brief leaves avaiable the segment to be used in futures calls to sysMalloc. The memory
 *  			segment is taken from the function pased by arguments.
 *
 * 		\param ap the logical memory segment to be freed.
 * 		\param getMemoryArea Function that returns the memory frame that sysMalloc wold be used in the future.
 * 		
 * 		\sa sysRealloc() sysMalloc()
 *
 */
void sysFree(void *ap, memArea_t * (*getMemoryArea)());

/**
 * \fn void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)())
 *
 * 		\brief Function that gives a continues segment of memory. This memory becames from the
 * 				function getMemoryArea. It could return null if there is not enougth free space
 *				on that frame.
 *
 * 		\param nbytes the size of continues segment memory.
 * 		\param getMemoryArea Functions that gives the frame to use be used by sysMalloc.
 * 		
 * 		\return The continues segment memory.
 *
 * 		\sa sysFree() sysRealloc()
 *
 */
void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)());

/**
 * \fn void *sysRealloc(void *ptr, size_t size, memArea_t * (*getMemoryArea)())
 *
 * 		\brief This function change the original size of the logical memory to a new one if
 *  				it's necesary.Returns null if it's not enought free space.
 *
 * 		\param ptr old logical segment memory, that could be moved to a new one.
 * 		\param size The new size of the continues segment memory.
 * 		\param getMemoryArea Functions that gives the frame to use be used by sysMalloc.
 * 		
 * 		\return The new continues segment memory.
 *
 * 		\sa sysFree() sysMalloc()
 *
 */
void *sysRealloc(void *ap, size_t size, memArea_t * (*getMemoryArea)());

#endif