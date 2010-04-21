/*
 * \file screensaver.c
 *
 * \brief 	It show a screensaver if nothing happen and dissapear if any interrypt
 * 			has occurred, it will dissappear.
 */

#include "screensaver.h"

char bufferShell[VIDEO_MEMORY_SIZE];
static int isFirstTime = 1;
int screenSaverTime = DEFAULT_SCREENSAVER_TIME;
static int rowBackup, colBackup,rowMouseBackup, colMouseBackup;

int screenSaver( void )
{
	static int colors[] = {0x01, 0x02, 0x03, 0x0F, 0x04, 0x05, 0x0F, 0x06, 0x01, 0x02};
	int status = LIVE;
	if( isFirstTime )
	{
		copyArea(0,0,SCREEN_WIDTH-1,SCREEN_HEIGTH-1, bufferShell);
		getCursorPosition(&rowBackup, &colBackup);
		getPointerPosition(&rowMouseBackup, &colMouseBackup);
		clearScreen();
		putColoredStringAtFixedPos("FLYING HIGH OS 1.0", colors, 13, SCREEN_HEIGTH/2, SCREEN_WIDTH/2 - 6);
		setCursorVisibility(0);
		setPointerVisibility(0);
		isFirstTime = 0;
		refreshScreen();
	}
	else
	{
		if( /*checkMouseActivity() ||*/ checkKeyboardActivity() )
		{
			pasteArea(CURSOR_START_COL,CURSOR_START_ROW,SCREEN_WIDTH-1,SCREEN_HEIGTH-1, bufferShell );
			setCursorVisibility(1);
			setPointerVisibility(1);
			setCursorPosition( rowBackup, colBackup );
			setPointerPosition(rowMouseBackup, colMouseBackup );
			isFirstTime = 1;
			status = DEAD;
		}
	}
	return status;
}


int getScreenSaverTime( void )
{
	return screenSaverTime;
}


void setScreenSaverTime( int time )
{
	screenSaverTime = time;
}

