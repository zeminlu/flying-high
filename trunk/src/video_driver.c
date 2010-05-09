/*
 *	File: video_driver.c
 *
 *		Brief Implementation of the video_driver.h file
 *
 * 		Author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
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

static int pointerCol = POINTER_START_COL;

static int pointerRow = POINTER_START_ROW;

static short pointerOffset = getOffset(POINTER_START_ROW, POINTER_START_COL);

static unsigned char pointerColor = POINTER_COLOR;

static int pointerVisibility = 0;

static unsigned char  oldPointedColor = VIDEO_COLOR;

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
		if ( i  == pointerOffset )
			oldPointedColor = videoColor;
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
	/*static printFunctionT specialCharFunctions[] = {
		printAlarm,
		printBackspace,
		printTab,
		printNewLine,
		printVTab,
		printLineFeed,
		printReturn
	};
	if ( '\a' <= c && c <= '\r' )
	{
		specialCharFunctions[c - '\a']();
		return 0;
	}
	else
	{
		videoBuffer[offset] = c;
		return 1;
	}*/
	videoBuffer[offset] = c;
	return 1;
}

static void pointerOn ( void  )
{
	if ( pointerVisibility )
	{
		oldPointedColor =  videoBuffer[pointerOffset + 1];
		videoBuffer[pointerOffset + 1] = pointerColor;
	}
}

static void pointerOff ( void )
{
	if ( pointerVisibility )
	{
		videoBuffer[pointerOffset + 1] = oldPointedColor;
	}
}

static int transverseArea ( int leftUpper_x, int leftUpper_y, int rightLower_x,
	int rightLower_y, void (*callback)(unsigned char *, unsigned char *,void *), void * callbackArgs )
{
	int i, j, offset;

	if ( leftUpper_x < 0 || leftUpper_x >= SCREEN_WIDTH || leftUpper_y < 0 || leftUpper_y >= SCREEN_HEIGTH ||
		 rightLower_x < 0 || rightLower_x >= SCREEN_WIDTH || rightLower_y < 0 || rightLower_y >= SCREEN_HEIGTH ||
		 leftUpper_y > rightLower_y || leftUpper_x > rightLower_x )
		return 0;
	
	for ( i = leftUpper_y; i <= rightLower_y; ++i )
		for ( j = leftUpper_x; j <=  rightLower_x; ++j )
		{
			offset = getOffset(i, j);
			if ( offset == pointerOffset  )
				callback(&(videoBuffer[offset]), &(oldPointedColor), callbackArgs);
			else
				callback(&(videoBuffer[offset]), &(videoBuffer[offset + 1]), callbackArgs);
		}
	return 1;
}

static void paintChar ( unsigned char * c, unsigned char * color, void * notUsed )
{
	*color = colorMask(*color);
}

typedef struct indexedBuffer {
	void * buffer;
	int index;
} indexedBufferT;

static void copyChar ( unsigned char * c, unsigned char * color, void * package )
{
	char * buffer = ((indexedBufferT *)package)->buffer;
	buffer[((indexedBufferT *)package)->index++] =  *c;
	buffer[((indexedBufferT *)package)->index++] = *color;
}

static void pasteChar ( unsigned char * c, unsigned char * color, void * package )
{
	char * buffer = ((indexedBufferT *)package)->buffer;
	*c = buffer[((indexedBufferT *)package)->index++];
	*color = buffer[((indexedBufferT *)package)->index++];
}

static void scrollScreen()
{
	pointerOff();
	memcpy(videoBuffer, videoBuffer + (SCREEN_WIDTH * 2), VIDEO_MEMORY_SIZE - (SCREEN_WIDTH * 2));
	pointerOn();
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

void initVideo ( int cursorEnabled, int pointerEnabled ) 
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
	/*setPointerVisibility(pointerEnabled);*/
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

/* Mouse Pointer Functions */

int setPointerPosition ( int row, int col )
{
	if ( row >= SCREEN_HEIGTH )
		pointerRow = SCREEN_HEIGTH - 1;
	else if ( row < 0 )
		pointerRow = 0;
	else
		pointerRow = row;

	if ( col >= SCREEN_WIDTH )
		pointerCol = SCREEN_WIDTH - 1;
	else if ( col < 0 )
		pointerCol = 0;
	else
		pointerCol = col;

	pointerOff();
	pointerOffset = getOffset(pointerRow, pointerCol);
	pointerOn();
	
	return pointerOffset;
}

int setPointerPositionByOffset ( int offset )
{
	if ( offset >= SCREEN_SIZE || offset < 0 )
		return -1;
	
	pointerOff();
	pointerOffset = offset % 2;
	pointerOn();
	
	return pointerOffset;
}

int getPointerPosition ( int * row, int * col )
{
	*row = pointerRow;
	*col = pointerCol;
	return pointerOffset;
}

int setPointerVisibility ( int b )
{
	if ( b && !pointerVisibility  )
	{
		pointerOn();
		pointerVisibility = !pointerVisibility;
	}
	else if ( !b && pointerVisibility )
	{
		pointerOff();
		pointerVisibility = !pointerVisibility;
	}
	return pointerVisibility;
}

int isPointerVisible ( void )
{
	return pointerVisibility;
}

int getPointerColor ( void )
{
	return pointerColor;
}

int setPointerColor ( int color )
{
	return pointerColor = color;
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

int paintArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y )
{
	return transverseArea(leftUpper_x, leftUpper_y, rightLower_x, rightLower_y, paintChar, NULL);
}

int copyArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y, char * buffer )
{
	indexedBufferT package;
	package.buffer = buffer;
	package.index = 0;
	return transverseArea(leftUpper_x, leftUpper_y, rightLower_x, rightLower_y, copyChar, &package);
}

int pasteArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y, char * buffer )
{
	indexedBufferT package;
	package.buffer = buffer;
	package.index = 0;
	return transverseArea(leftUpper_x, leftUpper_y, rightLower_x, rightLower_y, pasteChar, &package);
}

void putCharAtCurrentPos ( int c, int color )
{
	parseCharacter(c, cursorOffset);
	if ( cursorOffset != pointerOffset )
		videoBuffer[cursorOffset + 1] = videoColor;
	else
		oldPointedColor = color;
}

void printChar ( int c )
{
	printColorChar(c, videoColor);
}

void printColorChar ( int c, int color )
{
	int inc = parseCharacter(c, cursorOffset);
	if ( cursorOffset != pointerOffset )
		videoBuffer[cursorOffset + 1] = color;
	else
		oldPointedColor = color;
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
	if ( cursorOffset != pointerOffset )
		videoBuffer[i + 1] = videoColor;
	else
		oldPointedColor = videoColor;

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
		if ( cursorOffset != pointerOffset ) 
		{
			if ( color == NULL )
				videoBuffer[i] = videoColor;
			else
				videoBuffer[i] = color[j];
		}
		else
		{
			if ( color == NULL )
				oldPointedColor = videoColor;
			else
				oldPointedColor = color[j];
		}
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
