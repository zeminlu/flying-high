/**
 *	\file shell.h
 *
 *		\brief The shell process module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _SHELL_H_
#define _SHELL_H_

#include "stdio.h"
#include "video_driver.h"
#include "unistd.h"
#include "system.h"
#include "string.h"
#include "colors.h"
#include "types.h"
#include "process.h"
#include "uMalloc.h"
#include "bin.h"
#include "schedule.h"
#include "bttlship.h"

/**
 * \fn int shell ( void )
 *
 * 		\brief The function that runs the shell application
 *
 */
int shell ( void );

/**
 * \fn int initGlobalDataShell(void)
 *
 * 		\brief Initializes the data structures for the shells.
 *
 * 		\return 0 on success, -1 on error.
 *
 */
int initGlobalDataShell(void);

#endif
