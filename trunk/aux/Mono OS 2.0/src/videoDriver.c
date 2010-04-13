/* 
 * File: videoDriver.c
 * Author: Santiago Bermudez
 * Version: 02/06/2009 - 12:17
 * Description: The video controller manages all operations in the screen.
 * Provides management functions of the mouse, screen printing and storage
 * areas of the screen.
 */

/* 
 * Include project section 
 */

#include "videoDriver.h"
#include "fdTable.h"
#include "primitives.h"
#include "string.h"
#include "portsHandler.h"
#include "sysfilesystem.h"
#include "stdio.h"
//extern fd_t fdTable[10];

/* 
 * Define section
 */

#define SCREEN_COLS 80
#define SCREEN_ROWS 25
#define CRTC_DATA 0x3D5 
#define CRTC_ADDRESS 0x3D4 

static int screenBufferFD = stdtty;
//static int screenBufferFD  = stdbk;
/*
 * Static variables
 */
/* Buffer to write with attributes */
static unsigned char formatedBuffer[SCREEN_SIZE*2];
/* Screen buffer with attributes */
unsigned char screenBufAt[SCREEN_SIZE*2*2];
/* Screen buffer with attributes */
static unsigned char screen[SCREEN_SIZE*2];
/* Screen buffer with attributes */
static unsigned char screenMemory[SCREEN_SIZE*2];
/* An offset buffer */
static int offSetBuf;
/* An offset buffer */
static int saveOffSet;
/* An offset buffer */
static int saveOffSet1;
/* An enter flag */
static int enterScroll = FALSE;
/* Mouse's cursor Y axis position */
static int mouseY = SCREEN_ROWS/2;
/* Mouse's cursor X axis position */
static int mouseX = SCREEN_COLS;
/* Saves the format before the mouse */
static unsigned char noMouseFormat = NO_MOUSE_FORMAT;
/* Saves the mouse's format */
static unsigned char mouseAt;
/* Saves a column of spaces */
static unsigned char spaceCols[(SCREEN_COLS)*2];
/* The left limit of the screen */
static unsigned char leftLimit=0;

/*
 * Private functions
 */


int setVideoDriverBufferFD( int fd ){
	int aux = screenBufferFD;
	screenBufferFD = fd;
	return aux;
}


/* Function: cleanCursor
 * Description: removes the blinking cursor from the screen.
 */

static void cleanCursor ( void )
{
    	int temp;
	unsigned char cursorCleaner;	
 	temp = inb(CRTC_ADDRESS);
        outb(CRTC_ADDRESS, 0x0A);
        cursorCleaner = inb(CRTC_DATA);
        cursorCleaner |= 0x20;
        outb(CRTC_DATA, cursorCleaner);
        outb(CRTC_ADDRESS, temp);

}

/*
 * Function: formatString
 * Description: Gives format to the string, to print it on screen.
 * Parameters:
 * 	count: The size of the string.
 * 	buff: The string address.
 *	format: The format speified in formats table.	
 */
unsigned char * formatString( int count, const unsigned char * buff, unsigned char format){
	int i = 0;
	int j = 0;
	while( j <= count ){
		formatedBuffer[i] = buff[j];
		++i;
		formatedBuffer[i] = format;
		++i;
		++j;
	}
	return formatedBuffer;
}

/*
 * Function: writeOnScreen
 * Description: writes on sreen and screenBuffer.
 * Parameters: 
 *	buffer: The uffer to be written.
 *	count: The size.
 */
void writeOnScreen(int fd, const unsigned char * buffer, size_t count){
	_sys_video_write(fd, buffer, count);
//	if( fd == stdout)
		_sys_video_write(screenBufferFD, buffer, count);
}

/*
 * Function: setVideoOffSet
 * Description: Sets the video offset in a specified number.
 * Parameters:
 *	offSet: The new offset.
 */
extern openFileTable_t openFiles; 
void setVideoOffSet(int fd,  int offSet ){
	/*fdTable[stdout].offset = fdTable[screenBufferFD].offset = offSet;*/
	openFiles.files[fd].offset = offSet; 
//	if( fd == stdout )
		 openFiles.files[getRealFD(screenBufferFD)].offset = offSet;
}

/*
 * Function: getVideoOffSet
 * Description: Gives the actual video offset.
 */
int getVideoOffSet( int fd ) {
	/*return fdTable[stdout].offset;*/
	return openFiles.files[fd].offset;
}

/*
 * Function: moveVideoOffSet2
 * Description: Moves the video offset, with out scrolling.
 * Parameters: 
 * 	movement: The movement.
 */
void moveVideoOffSet2( int fd, int movement ){
//	if( fd == stdout )
		setVideoOffSet( fd, (getVideoOffSet(fd) + movement + SCREEN_SIZE*2) % (SCREEN_SIZE*2));
}

/*
 * Function: completeSpaces
 * Description: Fills a static buffer with spaces, and gives to it a standard 
 * format. And return this buffer.
 */
unsigned char * completeSpaces( void ){
	int i;
	for ( i = 0; i < (SCREEN_COLS); ++i)
		spaceCols[i] = ' ';
		memcpy(spaceCols, formatString(SCREEN_COLS, spaceCols, NO_MOUSE_FORMAT), (SCREEN_COLS)*2);
	return spaceCols;
}

/*
 * Function: mouseScroll
 * Description: Moves the mouse for a good screen scroll. And, returns if it's 
 * nesessary to scroll the mouse again.
 */
int mouseScroll(){
	int ret = 0;
	if ( mouseY == SCREEN_ROWS -1)
		ret = 1;
	else
		mouseSet(0, 1);
	mouseY -= 1;
	return ret;
}
extern fileTable_t fileTable;
/*
 * Function: moveVideoOffSet
 * Description: Moves the video offset, with scrolling.
 * Parameters: 
 * 	movement: The movement.
 */
void moveVideoOffSet( int fd, int movement ){
	int scrollAgain = FALSE;
	if ( movement < 0 /* && fd == stdout*/) 
		moveVideoOffSet2( fd, movement);

	for ( ; (getVideoOffSet(fd) < SCREEN_SIZE*2) && movement > 0; setVideoOffSet( fd ,getVideoOffSet(fd) + 1), --movement);
	if ( movement > 0 || getVideoOffSet(fd) > SCREEN_SIZE*2-2 ) {
		if ( mouseScroll() )
			scrollAgain = TRUE;
		setVideoOffSet(fd, SCREEN_COLS*2);
		memcpy(screenMemory, /*screenBufAt*/ fileTable.files[getRealFD(screenBufferFD)].address +SCREEN_COLS*2, (SCREEN_SIZE - SCREEN_COLS /*- 1*/)*2);
		setVideoOffSet(fd, 0);
		writeOnScreen(fd,  screenMemory, SCREEN_SIZE*2 - SCREEN_COLS*2/* - 2*/);
		setVideoOffSet(fd, SCREEN_SIZE*2 - SCREEN_COLS*2);
		writeOnScreen( fd, spaceCols, (SCREEN_COLS)*2);
		setVideoOffSet(fd, SCREEN_SIZE*2 - SCREEN_COLS*2);
	}
	if ( scrollAgain == TRUE ){
	mouseSet(0,1);
	}

}

/*
 * Public functions
 */

/*
 * Function: initializeScreen
 * Description: Initialize the screen, set the left limit, forbidding the erase 
 * before this limit.
 * Parameters:
 * 	leftBound: The left limit.
 * 	defaultFormat: The starting attribute.
 */
void initializeScreen( int leftBound, unsigned char defaultFormat ){
	defaultFormat++;
	leftLimit = leftBound;
	noMouseFormat = NO_MOUSE_FORMAT;
	clearScreen(stdout);
	completeSpaces();
	cleanCursor();
}

/*
 * Section: Mouse management.
 * Description: Provides all functions to manage the mouse in screen.
 */

/*
 * Function: turnOnMouse
 * Description: Turn on the mouse on screen. Set the format of the mouse's
 * cursor position. Initialize the cursor on screen's center.
 * Parameters:
 * 	mouseFormat: A format defined by the table formats.
 */
void turnOnMouse( unsigned char mouseFormat ){
/*	offSetBuf = getVideoOffSet();
	setVideoOffSet(mouseY*SCREEN_COLS*2+mouseX+1);
	writeOnScreen( &mouseFormat , 1);
	setVideoOffSet(offSetBuf);
	mouseAt = mouseFormat;
*/
}

/*
 * Function: mouseSet
 * Description: Moves the mouse's cursor on screen. If the movement is out of bounds,
 * puts the cursor in the correspondant limit.
 * Parameters:
 * 	dx: Movement on X axis.
 * 	dy: Movement on Y axis.
 */
void mouseSet( int dx, int dy){
/*ffSetBuf = getVideoOffSet();
	setVideoOffSet(mouseY*SCREEN_COLS*2+mouseX+1);
	writeOnScreen( &noMouseFormat, 1);
	mouseX+=2*dx;
	mouseY+=dy;
	if ( mouseX >= SCREEN_COLS*2)
		mouseX= (SCREEN_COLS-1)*2;
	else if ( mouseX < 0)
		mouseX = 0;
	if ( mouseY >= SCREEN_ROWS){
		mouseY= (SCREEN_ROWS-1);
	}
	else if ( mouseY < 0)
		mouseY = 0;
	setVideoOffSet(mouseY*SCREEN_COLS*2+mouseX+1);
	noMouseFormat = screenBufAt[getVideoOffSet()];
	writeOnScreen( &mouseAt , 1);
	setVideoOffSet(offSetBuf);
*/
}

/*
 * Function: getMouseX
 * Description: Returns cursor's position on X axis.
 */
int getMouseX( void ){
	return mouseX;
}

/*
 * Function: getMouseY
 * Description: Returns cursor's position on Y axis.
 */
int getMouseY( void ){
	return mouseY;
}

/*
 * Function: changeMouseFormat
 * Description: Set the cursor format.
 * Parameters:
 * 	mouseFormat: A format defined by the table formats.	
 */
void changeMouseFormat( unsigned char mouseFormat ){
	mouseAt = mouseFormat;
}

/*
 * Section: Storage management.
 * Description: Provides functions to bufferize screen areas.
 */

/*
 * Function: bufferizeScreen
 * Description: Writes in buffer the area framed.
 * Prameters:
 * 	lX: Lower X.
 * 	uX: Upper X.
 * 	lY: Lower Y.
 * 	uY: Upper Y.
 * 	buffer: Where the framed area is copied. 
 */ 

size_t bufferizeScreen( int lX, int uX, int lY, int uY, unsigned char * copyBuffer ){
	int i, j, mod;
	screenBufAt[SCREEN_SIZE*2-2] = ' ';
	for( i = 0, j = 0; i <= (SCREEN_SIZE-1)*2;++i, ++i){
		mod = i-((int)(i/(SCREEN_COLS*2)))*(SCREEN_COLS*2);
		if(mod>=lX && mod<=uX && i/(SCREEN_COLS*2)<=uY && i/(SCREEN_COLS*2)>=lY){
			copyBuffer[j++] = screenBufAt[i];
		}
	}
	return (size_t) j;
}

void saveScreen( int fd ){
	saveOffSet = getVideoOffSet(fd);
	setVideoOffSet(fd, 0);
	memcpy( screenMemory, screenBufAt, (SCREEN_SIZE-1)*2);
}

void restoreScreen( void ){
/*	setVideoOffSet(0);
	writeOnScreen(screenMemory, (SCREEN_SIZE-1)*2);
	setVideoOffSet(saveOffSet);	
*/
	
}

/*
 * Section: Screen printing.
 * Description: Provides functions to print on screen.
 */ 

/*
 * Function: videoDriverWrite
 * Description: Writes on the file descriptor an amount count, starting on 
 * the buffer.
 * Parameters:
 * 	fileDescriptor: A file descriptor.
 * 	buffer: Where data starts.
 * 	count: The amount to write.
 */

ssize_t videoDriverWrite( int fileDescriptor, const void * buffer, size_t count ){
	int untilBound = SCREEN_SIZE - getVideoOffSet(fileDescriptor)/2;
	unsigned char * forBuff;
	forBuff = formatString(count, buffer, NO_MOUSE_FORMAT );
	if ( count <= untilBound/*- 1*/){
		writeOnScreen(fileDescriptor, (const void *)forBuff, count*2 );
		moveVideoOffSet(fileDescriptor,2*count);
	}
	else {
		enterScroll = FALSE;
		videoDriverWrite( fileDescriptor, buffer, untilBound);
		videoDriverWrite( fileDescriptor, buffer + untilBound, count - untilBound); 
	}
	mouseSet(0,0);
	return (ssize_t)count;
}

/*
 * Function: videoDriverWriteAt
 * Description: Writes on the file descriptor an amount count, starting on 
 * the buffer.
 * Parameters:
 * 	fileDescriptor: A file descriptor.
 * 	buffer: Where data starts.
 * 	count: The amount to write.
 */

ssize_t videoDriverWriteAt( int fileDescriptor, const void * buffer, size_t count ){
	int untilBound = SCREEN_SIZE - getVideoOffSet(fileDescriptor)/2;
	if ( count <= untilBound +1/*- 1*/ ){
		writeOnScreen(fileDescriptor, (const void *)buffer, count );
		moveVideoOffSet(fileDescriptor, count);
	}
	else {
		videoDriverWriteAt( fileDescriptor, buffer, untilBound);
		videoDriverWriteAt( fileDescriptor, buffer + untilBound, count - untilBound ); 
	}
	mouseSet(0,0);
	return (ssize_t)count;
}

/*
 * Function: enterOnScreen
 * Description: Writes an enter on screen.
 */

void enterOnScreen( int fd ){
	int offCol;
	moveVideoOffSet( fd, SCREEN_COLS*2 );/*
	offCol = fdTable[stdout].offset % (SCREEN_COLS*2);
	fdTable[stdout].offset -= offCol;
	fdTable[screenBufferFD].offset -= offCol;*/
	offCol = openFiles.files[fd].offset % (SCREEN_COLS*2);
	openFiles.files[fd].offset -= offCol;
	openFiles.files[getRealFD(screenBufferFD)].offset -= offCol;
	enterScroll = TRUE;
}

/*
 * Function: backSpaceOnScreen
 * Description: Writes a backSpace on screen.
 */

int backSpaceOnScreen( int fd ){
	
/*	if ( getVideoOffSet() % (SCREEN_COLS*2)<= leftLimit && enterScroll == TRUE)
		return FALSE;
*/	setVideoOffSet(fd, getVideoOffSet(fd)-2);
	putchar(' ');
	setVideoOffSet(fd , getVideoOffSet(fd)-2);
	return TRUE;
}

void clearScreen( int fd ){
	int i;
	for( i=0; i < 2*SCREEN_SIZE; ++i){
		screen[i]=' ';
		screen[++i]= NO_MOUSE_FORMAT;
	}
	setVideoOffSet(fd, 0);
	writeOnScreen(fd,  screen, SCREEN_SIZE*2);
	setVideoOffSet(fd, 0);
}

/*
 * Function: screenColorArea
 * Description: Sets the format of framed area.
 * Prameters:
 * 	lX: Lower X.
 * 	uX: Upper X.
 * 	lY: Lower Y.
 * 	uY: Upper Y.
 * 	format: A format defined by the table formats.
 */ 

void screenColorArea( int lX, int uX, int lY, int uY, unsigned char format )
{	/*
	int i;
	
	saveOffSet1 = getVideoOffSet();
	setVideoOffSet(0);
	for( i = 1; i < (SCREEN_SIZE)*2;++i, ++i)
		if((i%(SCREEN_COLS*2)>=lX) && (i%(SCREEN_COLS*2)<=uX+2) && (i/(SCREEN_COLS*2)<=uY) && (i/(SCREEN_COLS*2)>=lY)){
			setVideoOffSet(i);
			write(screenBufferFD, &format, 1);
	}
	setVideoOffSet(0);
	write(stdout, screenBufAt, SCREEN_SIZE*2);
	setVideoOffSet(saveOffSet1);
	noMouseFormat = format;
*/
}
/*
 * Function: screenColor
 * Description: Sets the format on the screen.
 * Prameters:
 * 	format: A format defined by the table formats.
 */ 

void screenColor( unsigned char format ){
/*	screenColorArea(0, 2*SCREEN_COLS, 0, SCREEN_ROWS, format);
	mouseSet(0,0);
*/
}

