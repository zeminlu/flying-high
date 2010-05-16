/**
 *	\file exceptions.h
 *
 *		\brief Functions related with exception handling.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include "stdio.h"
#include "kasm.h"
#include "kc.h"
#include "ttys.h"

/*
 *	Loads all the exception handlers
 *	into the IDT.
 */
/**
 * \fn void loadExceptionHandlers()
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void loadExceptionHandlers();

#endif
