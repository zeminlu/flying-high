/**
 *	\file shell.h
 *
 *		\brief
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
 * 		\brief Brief.
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
int initGlobalDataShell(void);

#endif
