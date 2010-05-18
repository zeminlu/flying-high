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
 * 		\brief Brief.
 *
 * 		\param ap ParamBrief.
 * 		\param getMemoryArea ParamBrief.
 * 		
 * 		\sa sysRealloc() sysMalloc()
 *
 */
void sysFree(void *ap, memArea_t * (*getMemoryArea)());

/**
 * \fn void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)())
 *
 * 		\brief Brief.
 *
 * 		\param nbytes ParamBrief.
 * 		\param getMemoryArea ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		\sa sysFree() sysRealloc()
 *
 */
void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)());

/**
 * \fn void *sysRealloc(void *ptr, size_t size, memArea_t * (*getMemoryArea)())
 *
 * 		\brief Brief.
 *
 * 		\param ptr ParamBrief.
 * 		\param size ParamBrief.
 * 		\param getMemoryArea ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		\sa sysFree() sysMalloc()
 *
 */
void *sysRealloc(void *ap, size_t size, memArea_t * (*getMemoryArea)());

#endif