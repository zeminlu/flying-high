/*
 *	File system.c
 *
 *		Brief:
 *
 *		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */
#include "system.h"

static int runningApp = TERMINAL, oldApp = TERMINAL;
unsigned int waitCounter = 0;


void rebootSystem(void)
{
	unsigned char good = 0x02;
	while ((good & 0x02) != 0)
        good = inportb(0x64);
    outportb(0x64, 0xFE);
}


static int terminal ( void )
{
	int c;

	if ( (c = getchar()) != EOF )
		putchar(c);
	
	return LIVE;
}

static proccessT applications[] = {
	terminal,
	shell,
	screenSaver
};

void launchApp ( int app )
{
	oldApp = runningApp;
	runningApp = app;

	return;
}

int isAppRunning(int app)
{
	return app == runningApp;
}

void runApp (void)
{ 
	if (!(applications[runningApp]()))
	{
		if(runningApp == SCREEN_SAVER)
			applications[runningApp = oldApp]();
		else
			applications[runningApp = TERMINAL]();
	}
}

void wait ( int miliseconds )
{
	waitCounter = miliseconds / MILISECOND_PER_TICKS;
}
