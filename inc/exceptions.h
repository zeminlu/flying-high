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
 * 		\brief Load the handler of the exception that
 *				was produced.
 *
 * 		Use:
 * 		\code
 *			
 *		\endcode
 *
 * 		\sa
 *
 */
void loadExceptionHandlers();

#endif
