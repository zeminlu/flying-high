/**
 *	\file kMalloc.h
 *
 *		\brief Brief.
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
 * 		\brief Brief.
 *
 * 		\param ap ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void kFree(void *ap);

/**
 * \fn void *kMalloc(size_t nbytes)
 *
 * 		\brief Brief.
 *
 * 		\param nbytes ParamBrief.
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
void *kMalloc(size_t nbytes);

/**
 * \fn void *kRealloc(void *ap, size_t size)
 *
 * 		\brief Brief.
 *
 * 		\param ap ParamBrief.
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
void *kRealloc(void *ap, size_t size);

#endif