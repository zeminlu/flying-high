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
 * 		\brief Brief.
 *
 * 		\param ap ParamBrief.
 * 		
 * 		\sa malloc() realloc()
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
 * 		\sa free() realloc()
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
 * 		\sa free malloc()
 *
 */
void *realloc(void *ap, size_t size);

#endif