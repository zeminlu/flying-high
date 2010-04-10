#include "video_driver.h"
#include "video_adapter.h"

void bufferMapper ( FILE *f/*, FILE *att*/ )
{
	char c;

	while ( (c = fgetc(f)) != EOF )
		printChar(c);/*printColorChar(c,fgetc(att));*/
}

void flushStdScreen (void)
{
	bufferMapper(stdout /*, outatt*/);
	refreshScreen();
}
