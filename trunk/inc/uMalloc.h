/**
 *	\file uMalloc.h
 *
 *		\brief Brief.
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
void free(void *ap);

/**
 * \fn void *malloc(size_t nbytes)
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
void *malloc(size_t nbytes);

/**
 * \fn void *realloc(void *ap, size_t size)
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
void *realloc(void *ap, size_t size);

#endif