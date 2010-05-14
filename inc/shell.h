/**
 *	\file shell.h
 *
 *		\brief
 *
 *		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
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
#include "video_adapter.h"
#include "types.h"
#include "process.h"
#include "uMalloc.h"
#include "bin.h"
#include "schedule.h"

/**
 * \fn int shell ( void )
 *
 * 		\brief The function that runs the shell application
 *
 */
int shell ( void );

int initGlobalDataShell(void);

#endif
