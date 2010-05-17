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
 * 		\brief This functions initialize the process welcome,
 *				all of shells and goddbye process. It you kill the process
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
 * 		\brief This function just print the welcome message to
 *				Flying-High SO
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
 * 		\brief This function just said goddbye and reboot the system.
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
 * 		\brief This functions print A letter to the infinity
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
 * 		\brief This functions print B letter to the infinity
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
 * 		\brief This function only consume the microprocessor
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
 * 		\brief This function create a page fault exception
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