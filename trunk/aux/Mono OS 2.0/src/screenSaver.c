/* 
 *  File: screenSaver.c
 *  Author: Lucas Pizzagalli
 *  Version: 02/06/2009 - 16:17
 *  Description: 
 *  		This file, contains the screen saver and all functions that
 *  		it involucrate.
 */
/* 
 *  Include project section 
 */  

#include "screenSaver.h"

/*
 * private global variables
 *
 */
int screenSaverOn;
unsigned int screenSaverTime = DEFAULT_SCREEN_SAVER_TIME;
unsigned int screenSaverActualTime = 0;

/*
 * screenSaver:
 * 		Description:
 * 			This program execute the screen saver 
 */

void
screenSaver( void )
{
	if(screenSaverOn)
	{
		if(screenSaverActualTime >= screenSaverTime)
			;
		else
			endScreenSaver();
	}
	else
	{
		if(screenSaverActualTime >= screenSaverTime)
			startScreenSaver();
		else
			++screenSaverActualTime;
	}
	return;
}

/*
 * endScreenSaver:
 * 		This function end the screen saver, 
 *		restart the time counter., and restore
 *		previous screen.
 *
 */

void
endScreenSaver( void )
{
	screenSaverActualTime = 0;
	screenSaverOn = OFF;
	restoreScreen();
	return;
}
	
/*
 * startScreenSaver:
 * 		Interupt and internal function to start screen saver.
 * 		It saves the actual screen and clear the screen.
 *		If you want a funnyer screen saver, HERE is where yo 
 *		have to change
 */
void
startScreenSaver( void )
{
	saveScreen(stdout);
	clearScreen(stdout);
	screenSaverOn = ON;
	return;
}
/*
 * setScreenSaver:
 * 		set a time in secons(aprox) in wich you wish
 *		screen saver to start when PC is inactive
 * recives:
 * 		UNSIGNED INT with the amount of seconds.
 */

void
setScreenSaverTime( unsigned int time )
{
	if(time > (4000000)/18)				/*MAX UNSIGNED INT*/
		screenSaverTime = time;
	else
		screenSaverTime = time*18;
	return;
}

/*
 * setScreenSaverActualTime:
 * 		modifies the screensaver time counter. It recives
 * 		the time in amount of interruptions
 * 
 */
static void
setScreenSaverActualTime( unsigned int time )
{
	screenSaverActualTime = time;
	return;
}
/*
 * turnOnScreenSaver:
 * 		This function is a function for the user to
 * 		turn on the screen saver, not taking in acount the
 * 		inactivity time
 *
 */
void
turnOnScreenSaver()
{
	setScreenSaverActualTime(screenSaverTime);
	startScreenSaver();
	return;
}
/*
 * activityDetected:
 * 		This function is meant to be called by the mouse and keyboard
 * 		interuption in order to restar the time counter of the screen saver
 *
 */
void
activityDetected( void )
{
	screenSaverActualTime = 0;
	if(screenSaverOn)
		endScreenSaver();
	return;
}
