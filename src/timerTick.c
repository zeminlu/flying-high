/*
 *	File: timerTick.c
 *
 *		Brief:
 *
 *		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */

#include "timerTick.h"

extern int waitCounter;

void timerTick ( void )
{
	/*static int mouseRefreshTime = 0;
	static int screenSaverCount = 0;*/

	/* Wait Counter Loop */
	if ( waitCounter != 0 )
	{
		while ( waitCounter-- );
		waitCounter = 0;
	}

	/* Mouse Postion Update */
	/*if ( mouseRefreshTime == 2 )
	{
		mouseRefreshTime = 0;
		refreshMouse();
	}
	else
		++mouseRefreshTime;
	*/
	/* Screen Update from Buffers */
	 /* flush kb buffer into stdin */
	/*refreshTTY();	 flush stdout into video buffer and refresh screen */

	/* Task Scheduler Decision */
	/*if (!isAppRunning(SCREEN_SAVER))
	{
		if( checkKeyboardActivity() || checkMouseActivity() )
			screenSaverCount = 0;
		else if( screenSaverCount == getScreenSaverTime() )
		{
			launchApp(SCREEN_SAVER);
			screenSaverCount = 0;
		}
		else
			++screenSaverCount;
		
	}*/
}