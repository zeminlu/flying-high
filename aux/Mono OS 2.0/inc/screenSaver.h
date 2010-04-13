/* 
 *  File: screenSaver.h
 *  Author: Lucas Pizzagalli
 *  Version: 02/06/2009 - 16:17
 *  Description: 
 *  		This file, contains the screen saver and all functions that
 *  		it involucrate.
 */

#ifndef SCREENSAVER_H_
#define SCREENSAVER_H_
/* 
 *   Include project section 
 */   

#include "videoDriver.h"

/* 
 *  Define section
 */ 

#define DEFAULT_SCREEN_SAVER_TIME 30000
#define ON 1
#define OFF 0
/*
 * private global variables
 *
 */

/*
 * screenSaver:
 * 		Description:
 * 			This program execute the screen saver 
 */

void screenSaver( void );

/*
 * endScreenSaver:
 * 		This function end the screen saver and 
 *		restart the time counter.
 *
 */

void endScreenSaver( void );
	
/*
 * startScreenSaver:
 * 		Interupt and internal function to start screen saver.
 * 		It saves the actual screen and clear the screen.
 *		If you want a funnyer screen saver, HERE is where yo 
 *		have to change
 */
void startScreenSaver( void );
/*
 * setScreenSaver:
 * 		set a time in secons(aprox) in wich you wish
 *		screen saver to start when PC is inactive
 * recives:
 * 		UNSIGNED INT with the amount of seconds.
 */

void setScreenSaverTime( unsigned int time );

/*
 * turnOnScreenSaver:
 * 		This function is a function for the user to
 * 		turn on the screen saver, not taking in acount the
 * 		inactivity time
 *
 */
void turnOnScreenSaver();
/*
 * activityDetected:
 * 		This function is meant to be called by the mouse and keyboard
 * 		interuption in order to restar the time counter of the screen saver
 *
 */
void activityDetected( void );

#endif
