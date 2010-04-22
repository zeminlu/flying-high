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

#include "config.h"
#include "stdio.h"
#include "video_driver.h"
#include "unistd.h"
#include "system.h"
#include "string.h"
#include "colors.h"
#include "screensaver.h"
#include "video_adapter.h"

/**
 * \fn int shell ( void )
 *
 * 		\brief The function that runs the shell application
 *
 */
int shell ( void );

#endif