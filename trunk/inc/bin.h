/**
 *	\file bin.h
 *
 *		\brief All the OS processes.
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
 * 		\brief This functions initialize the welcome process,
 *				all the shells and goodbye process. If you kill the process
 *				init the system will be rebooted.
 *
 * 		\param args 
 * 		
 * 		Use:
 * 		\code
 *			initMultitasker(init);
 *		\endcode
 *
 */
void init(void *args);

/**
 * \fn void top(char *args)
 *
 * 		\brief This function show what percentage of the microprocessor
 *				it is using each process. When you write the command top
 *				in the shell, the process top will be created.
 *
 * 		\param args
 * 		
 * 		Use:
 * 		\code
 *			createProcess("top", (void(*)(void *))top, NULL, FOREGROUND);
 *		\endcode
 *
 */
void top(char *args);

/**
 * \fn  void welcome()
 *
 * 		\brief This function just prints the welcome message to
 *				Flying-High OS
 *
 * 		Use:
 * 		\code
 *			createProcess("welcome", (void(*)(void *))welcome, NULL, FOREGROUND)
 *		\endcode
 *
 * 		\sa
 *
 */
void welcome();

/**
 * \fn void goodbye()
 *
 * 		\brief This function just says goodbye and reboots the system.
 *
 * 		Use:
 * 		\code
 *			createProcess("goodbye", (void(*)(void *))goodbye, NULL, FOREGROUND);
 *		\endcode
 *
 * 		\sa
 *
 */
void goodbye();

/**
 * \fn void printA()
 *
 * 		\brief This function prints infinite A letters
 *
 * 		Use:
 * 		\code
 *			createProcess("printA", (void(*)(void *))printA, NULL, FOREGROUND)
 *		\endcode
 *
 * 		\sa
 *
 */
void printA();

/**
 * \fn void printB()
 *
 * 		\brief This functions prints infinite A letters
 *
 * 		Use:
 * 		\code
 *			createProcess("printB", (void(*)(void *))printB, NULL, FOREGROUND)
 *		\endcode
 *
 * 		\sa
 *
 */
void printB();

/**
 * \fn void nothing()
 *
 * 		\brief This function only consumes the microprocessor
 *
 * 		Use:
 * 		\code
 *			createProcess("nothing", (void(*)(void *))nothing, NULL, FOREGROUND)
 *		\endcode
 *
 * 		\sa
 *
 */
void nothing();

/**
 * \fn void pageFault()
 *
 * 		\brief This function generates a page fault exception
 * 		
 * 		Use:
 * 		\code
 *			createProcess("pageFault", (void(*)(void *))pageFault, NULL, FOREGROUND)
 *		\endcode
 *
 * 		\sa
 *
 */
void pageFault();

#endif