/**
 *	\file video_driver.c
 *
 *		\brief Implementation of video_driver.h file.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

/*
 *	Include Section
 */
#include "video_driver.h"

/*
 *	Macro Definition Section
 */
#define VGA_MISC_REGISTER	0x3CC
#define VGA_CURSOR_POS_HIGH_PORT 0x0E
#define VGA_CURSOR_POS_LOW_PORT 0x0F
#define VGA_MAX_SCAN_LINE_REGISTER 0x09
#define VGA_CURSOR_START_REGISTER 0x0A
#define VGA_CURSOR_END_REGISTER 0x0B
#define VGA_INPUT_STATUS_REGISTER 0X3DA
#define VGA_ADDRESS_DATA_REGISTER 0X3C0
#define VGA_DATA_READ_REGISTER 0x3C1
#define colorMask(c)			((c << 4)  + (c >> 4))
#define getOffset(row,col) 2 * ( (row) * SCREEN_WIDTH + (col) )

/*
 *	Screen Properties Definition
 */


static short cursorOffset = getOffset(CURSOR_START_ROW, CURSOR_START_COL);

static int cursorRow = CURSOR_START_ROW;

static int cursorCol = CURSOR_START_COL;

static int cursorVisibility = 1;

static unsigned char * videoMemory = VIDEO_MEMORY_ADDRESS;

static unsigned char videoBuffer[VIDEO_MEMORY_SIZE];

static unsigned char videoColor = VIDEO_COLOR;

static int CRTCAddress, CRTCData;

static int vtabStop = VIDEO_VTAB_STOP;

static int tabStop = VIDEO_TAB_STOP;

typedef void (*printFunctionT)(void);

/*
 *	Private Function Section
 */


static void fillLine ( int c )
{
	int i;
	for ( i = cursorOffset; i < VIDEO_MEMORY_SIZE; i += 2 )
	{
		videoBuffer[i] = c;
		videoBuffer[i + 1] = videoColor;
	}
}

void printAlarm ( void )
{
}

void printBackspace ( void )
{
	decCursor();
	videoBuffer[cursorOffset] = ' ';
	videoBuffer[cursorOffset + 1] = videoColor;
}

void printNewLine ( void )
{
	do
	{
		incCursor();
	} while ( cursorCol != 0 );
}

void printLineFeed ( void )
{
	int col;

	printNewLine();
	col = cursorCol;
	do
	{
		incCursor();
	} while ( cursorCol == col );
}

void printReturn ( void )
{
	while ( cursorCol != 0 )
		decCursor();
}

void printTab ( void  )
{
	int i, prevRow;


	for ( i = 0, prevRow = cursorRow; i < tabStop; ++i )
	{
		if ( prevRow != cursorRow )
			break;
		videoBuffer[cursorOffset] = ' ';
		videoBuffer[cursorOffset + 1] = videoColor;
		incCursor();
	}
}

void printVTab ( void )
{
	int i, col;

	col = cursorCol - 1;
	for ( i = 0; i < vtabStop; ++i )
		printNewLine();
	while ( cursorCol < col )
		incCursor();
}



static int parseCharacter ( int c, int offset )
{
	videoBuffer[offset] = c;
	return 1;
}

typedef struct indexedBuffer {
	void * buffer;
	int index;
} indexedBufferT;

static void scrollScreen()
{
	memcpy(videoBuffer, videoBuffer + (SCREEN_WIDTH * 2), VIDEO_MEMORY_SIZE - (SCREEN_WIDTH * 2));
	cursorRow = SCREEN_HEIGTH -1;
	cursorCol = 0;
	cursorOffset = getOffset(cursorRow, cursorCol);
	fillLine(' ');
}

static void refreshCursor ( void )
{
	int temp;

	temp = inportb(CRTCAddress);

	outportb(CRTCAddress, VGA_CURSOR_POS_LOW_PORT);
	outportb(CRTCData, cursorOffset/2);
	outportb(CRTCAddress, VGA_CURSOR_POS_HIGH_PORT);
	outportb(CRTCData, (cursorOffset/2) >> 8);
	
	outportb(CRTCAddress, temp);
}


/*
 *	Public Function Section
 */

void initVideo ( int cursorEnabled) 
{
	int temp, temp2, miscVGARegister, CRTControler, cursorStart,
		cursorEnd, maxScanLineRegister;
	/*
	 *	Disabling blink-bit
	 */

	inportb(VGA_INPUT_STATUS_REGISTER);
	temp = inportb(VGA_ADDRESS_DATA_REGISTER);
	outportb(VGA_ADDRESS_DATA_REGISTER, 0x10);
	temp2 = inportb(VGA_DATA_READ_REGISTER);
	temp2 &= 0xF7;
	outportb(VGA_ADDRESS_DATA_REGISTER, temp2);
	outportb(VGA_ADDRESS_DATA_REGISTER, temp);
	inportb(VGA_INPUT_STATUS_REGISTER);

	/*
	 *	Getting VGA CRT Control registers Address
	 */
	miscVGARegister = inportb(VGA_MISC_REGISTER);
	miscVGARegister &= 0x01; /*getting I/O Address select bit*/
	if ( miscVGARegister == 0x01 )
		CRTControler = 0x03D0;
	else
		CRTControler = 0x03B0;

	/*
	 *	Initializing CRT Control Address and Data
	 */
	CRTCData = CRTControler + 5;
	CRTCAddress = CRTControler + 4;

	/*
	 * Defining Cursor Shape 
	 */
	temp = inportb(CRTCAddress);
	
	outportb(CRTCAddress, VGA_MAX_SCAN_LINE_REGISTER);
	maxScanLineRegister = inportb(CRTCData);
	maxScanLineRegister &= 0x1F;
	outportb(CRTCAddress, VGA_CURSOR_START_REGISTER);
	cursorStart = inportb(CRTCData);
	cursorStart &= 0xE0;
	outportb(CRTCData, cursorStart);
	outportb(CRTCAddress, VGA_CURSOR_END_REGISTER);
	cursorEnd = inportb(CRTCData);
	cursorEnd &= 0xE0;
	cursorEnd += maxScanLineRegister;
	outportb(CRTCData, cursorEnd);

	outportb(CRTCAddress, temp);
	
	setCursorVisibility(cursorEnabled);
	refreshCursor();
}

void refreshScreen ( void ) {
	memcpy(videoMemory, videoBuffer, VIDEO_MEMORY_SIZE);
	refreshCursor();
}

void updateColor ( void )
{
	int i;
	for ( i = 0; i < VIDEO_MEMORY_SIZE; i += 2 ) 
		videoBuffer[i+1] = videoColor;
}
/* Cursor Movment Functions */

int setCursorVisibility ( int b )
{
	int temp, cursorDisabled;

	if ( b & !cursorVisibility )
	{
		temp = inportb(CRTCAddress);
		outportb(CRTCAddress, 0x0A);
		cursorDisabled = inportb(CRTCData);
		cursorDisabled &= 0xDF;
		outportb(CRTCData, cursorDisabled);
		outportb(CRTCAddress, temp);
		cursorVisibility = !cursorVisibility;
	}
	else if ( (!b) & (cursorVisibility) )
	{
		temp = inportb(CRTCAddress);
		outportb(CRTCAddress, 0x0A);
		cursorDisabled = inportb(CRTCData);
		cursorDisabled |= 0x20;
		outportb(CRTCData, cursorDisabled);
		outportb(CRTCAddress, temp);
		cursorVisibility = !cursorVisibility;
	}

	return cursorVisibility;
}

void incCursor ( void )
{
	cursorOffset += 2;
	++cursorCol;
	if ( cursorCol == SCREEN_WIDTH )
	{
		cursorCol = 0;
		++cursorRow;
	}
	if ( cursorRow == SCREEN_HEIGTH )
		scrollScreen();
}

void decCursor ( void )
{
	if ( (cursorOffset -= 2) < 0 )
		cursorOffset = 0;
	
	--cursorCol;
	if ( cursorCol == -1 )
	{
		cursorCol = SCREEN_WIDTH - 1;
		--cursorRow;
	}
	if ( cursorRow == -1 )
	{
		cursorRow = 0;
		cursorCol = 0;
	}
}

int setCursorPosition ( int row, int col )
{
	if ( row >= SCREEN_HEIGTH || col >= SCREEN_WIDTH || row < 0 || col < 0 )
		return -1;
	cursorOffset = getOffset(row, col);
	cursorRow = row;
	cursorCol = col;
	return cursorOffset;
}

void getCursorPosition ( int * row, int * col )
{
	if ( row == NULL || col == NULL )
		return;
	*row = cursorRow;
	*col = cursorCol;
}


/*	Screen Properties Getters & Setters */

int getVideoColor ( void )
{
	return videoColor;
}

int setVideoColor ( int color )
{
	return videoColor = color;	
}

int getVideoTabStop ( void )
{
	return tabStop;
}

int setVideoTabStop ( int num ) 
{
	if ( num <= 0 )
		return -1;
	return tabStop = num;
}

int getVideoVTabStop ( void )
{
	return vtabStop;
}

int setVideoVTabStop ( int num )
{
	if ( num <= 0 )
		return -1;
	return vtabStop = num;
}
/* Video Driver Functions */

void putCharAtCurrentPos ( int c, int color )
{
	parseCharacter(c, cursorOffset);
	videoBuffer[cursorOffset + 1] = videoColor;
}

void printChar ( int c )
{
	printColorChar(c, videoColor);
}

void printColorChar ( int c, int color )
{
	int inc = parseCharacter(c, cursorOffset);
	videoBuffer[cursorOffset + 1] = color;
	if ( inc )
		incCursor();
}

int putCharAtFixedPos ( int c, int color, int row, int col )
{
	int i;

	if ( col >= SCREEN_WIDTH || row >= SCREEN_HEIGTH || col < 0 || row < 0 )
		return 0;
	
	i = getOffset(row, col);
	parseCharacter(c, cursorOffset);
	videoBuffer[i + 1] = videoColor;
	
	return 1;
}

int putColoredStringAtFixedPos ( char * s, int * color, int length, int row, int col  )
{
	int i, j;

	if ( col >= SCREEN_WIDTH || row >= SCREEN_HEIGTH || col < 0 || row < 0 )
		return -1;
	
	for ( i = getOffset(row, col), j = 0; j < length && j < VIDEO_MEMORY_SIZE; ++i, ++j )
	{
		parseCharacter(s[j], i++);
		if ( color == NULL )
			videoBuffer[i] = videoColor;
		else
			videoBuffer[i] = color[j];
	}

	return j;
}

void fillScreen ( int c )
{
	int i;

	for ( i = 0; i < VIDEO_MEMORY_SIZE; i += 2 ) 
	{
		videoBuffer[i] = c;
		videoBuffer[i+1] = videoColor;
	}
		
}

void clearScreen ( void )
{
	fillScreen(' ');
}
