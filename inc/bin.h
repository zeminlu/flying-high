/**
 *	\file bin.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _BIN_H_
#define _BIN_H_

#include "sysProcess.h"
#include "stdio.h"
#include "shell.h"
#include "string.h"
#include "process.h"

/**
 * \fn void init(void *args)
 *
 * 		\brief Brief.
 *
 * 		\param args ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void init(void *args);

/**
 * \fn void top(char *args)
 *
 * 		\brief Brief.
 *
 * 		\param args ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void top(char *args);

/**
 * \fn  void welcome()
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
void welcome();

/**
 * \fn void goodbye()
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
void goodbye();

/**
 * \fn void printA()
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
void printA();

/**
 * \fn void printB()
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
void printB();

/**
 * \fn void nothing()
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
void nothing();

/**
 * \fn void pageFault()
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
void pageFault();

#endif