/*
 *	\file screensaver.h
 *
 */
#ifndef _SCREENSAVER_H_
#define _SCREENSAVER_H_

#include "config.h"
#include "video_driver.h"
#include "stdio.h"
#include "keyboard_driver.h"
#include "system.h"

/*
 *	\fn ins screenSaver(void)
 *
 *	\brief 	This function act after a certain time without any keyboard
 *			and mouse interruption. It clear the screen and put a simple message
 *			like this "ARK OS 1.0". When it recieve the order that someone periferic
 *			had interrupted, the screensaver disappear.
 *
 */

int screenSaver( void );

/*
 * \fn int getScreenSaverTime(void)
 *
 * \brief 	Return the time that the screensaver is waiting to
 * 			be showed.
 */

int getScreenSaverTime(void);

/*
 * \fn void setScreenSaverTime( int )
 *
 * \brief Modify the screensaver time. Set the new time.
 */

void setScreenSaverTime(int num);

#endif
