/**
 *	\file sysMalloc.h
 *
 *		\brief Brief.
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
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
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
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)());

/**
 * \fn void *sysRealloc(void *ap, size_t size, memArea_t * (*getMemoryArea)())
 *
 * 		\brief Brief.
 *
 * 		\param ap ParamBrief.
 * 		\param size ParamBrief.
 * 		\param getMemoryArea ParamBrief.
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
void *sysRealloc(void *ap, size_t size, memArea_t * (*getMemoryArea)());

#endif