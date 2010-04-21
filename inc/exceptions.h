/*
 *	exceptions.h
 *
 *		Functions related with exception handling.
 *
 *		Author: Luciano Zemin
 *
 */

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include "stdio.h"
#include "kasm.h"
#include "kc.h"

/*
 *	Loads all the exception handlers
 *	into the IDT.
 */
void loadExceptionHandlers();

#endif
