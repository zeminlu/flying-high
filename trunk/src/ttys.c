/*
 * ttys.c
 */

#include "ttys.h"

static int offset = 0;

typedef struct TTY{
	tty_t			ttyId;
	Keycode *		stdIn;
	unsigned char 	TerminalBuffer[VIDEO_MEMORY_SIZE];
	unsigned char 	*begin, *end;
	int				hasScrolled;
	pid_t			focusProcess;
}TTY;

typedef struct sysTTY{
	int		qtyTTY;
	TTY		ttys[MAX_TTY];
	tty_t	focusTTY;
}sysTTY;

typedef struct stdInTTY{
	Keycode stdIN[MAX_LINE];
	int 	writeOffset;
	int 	readOffset;
	int 	empty;
	}stdInTTY;

typedef void (*printFunctions)(unsigned char *, int, tty_t);

sysTTY ttyTable;

static stdInTTY stdinTableTTY[MAX_TTY];

static int sleepCondition[MAX_TTY]= {0};

static char * promnt = "root@FlyingHigh$>";

/*
 *	Functions Section
 */

/*
 *	Static functions for putcharTTY
 */


static void putLine( tty_t tty)
{
	switch( runningProcess->ttyMode )
	{
		case TTY_RAW:

			break;
		case TTY_CANONICAL:
			sleepCondition[tty]++;
			break;
	}
}

static void parseAlarmTTY( unsigned char * p, int where, tty_t tty )
{
	printAlarm();	
}

static void parseBackSpaceTTY( unsigned char * p, int where, tty_t tty )
{
	if( !where )
	{
		if( offset > 0 ){
			--p;
			*p++ = ' ';
		}
	}
	return;
}

static void parseTabTTY(unsigned char * p, int where, tty_t tty )
{
	if( !where ) 
		*p = TAB;
	else 
		printTab();
	p += VIDEO_TAB_STOP;
	offset += VIDEO_TAB_STOP;	
}

static void parseNewLineTTY( unsigned char * p, int where, tty_t tty )
{
	if( !where ) 
		*p = NEW_LINE;
	else 
		printNewLine();

	p += SCREEN_WIDTH - (offset % SCREEN_WIDTH);
	offset += SCREEN_WIDTH - (offset % SCREEN_WIDTH);
}

static void parseVTabTTY( unsigned char * p, int where, tty_t tty )
{
	int i, curOffset;
	
	if( !where ) 
		*p = VTAB;
	else 
		printVTab();
	curOffset = offset % SCREEN_WIDTH;
	for( i = 0 ; i < VIDEO_VTAB_STOP ; ++i )
	{
		parseNewLineTTY(p,where, tty);
	}
	while( curOffset >= offset)
	{
		++offset;
		++p;
	}
}

static void parseLineFeedTTY( unsigned char * p, int where, tty_t tty )
{
	int curOffset;
	
	putLine( tty);
	parseNewLineTTY(p,where, tty);
	printNewLine();
	curOffset = offset % SCREEN_WIDTH;
	do
	{
		++p;
		++offset;
	}while( curOffset-- >= 0 );
}

static void parseReturnTTY( unsigned char * p, int where, tty_t tty )
{
	printReturn();
	while( (offset % SCREEN_WIDTH) != 0 )
	{
		--p;
		--offset;
	}
}

static int parseCharTTY( int c, tty_t tty )
{
	static printFunctions specialCharPrint[] = {
		parseAlarmTTY,
		parseBackSpaceTTY,
		parseTabTTY,
		parseNewLineTTY,
		parseVTabTTY,
		parseLineFeedTTY,
		parseReturnTTY
	};
	
	if( '\a' <= c && c >= '\r' )
	{
		specialCharPrint[c - '\a'](ttyTable.ttys[getCurrentTTY()].begin, WRITE_ON_TTY, tty);
		return 0;
	}else
	{
		*(ttyTable.ttys[getCurrentTTY()].begin) = c;
		return 1;
	}
}

static void refreshTTYScreen( void )
{
	int bckOffset, character;
	static printFunctions refreshCharPrint[] = {
		parseAlarmTTY,
		parseBackSpaceTTY,
		parseTabTTY,
		parseNewLineTTY,
		parseVTabTTY,
		parseLineFeedTTY,
		parseReturnTTY
	};
	
	bckOffset = offset;
	offset = 0;
	
	if( ttyTable.ttys[getCurrentTTY()].hasScrolled > 0 )
	{
		ttyTable.ttys[getCurrentTTY()].end = ttyTable.ttys[getCurrentTTY()].begin + (bckOffset % SCREEN_WIDTH) + 1;
		offset = (ttyTable.ttys[getCurrentTTY()].begin - ttyTable.ttys[getCurrentTTY()].TerminalBuffer ) + SCREEN_WIDTH;
	}
	while( ttyTable.ttys[getCurrentTTY()].end != ttyTable.ttys[getCurrentTTY()].begin )
	{
		if( offset == VIDEO_MEMORY_SIZE )
			offset = 0;
		character = *(ttyTable.ttys[getCurrentTTY()].end);
		if( '\a' <= character && character >= '\r' )
		{
			refreshCharPrint[character - '\a'](ttyTable.ttys[getCurrentTTY()].end, SEND_TO_VIDEO, getCurrentTTY());
		}else
		{
			putCharAtCurrentPos((int)(*(ttyTable.ttys[getCurrentTTY()].end)), getVideoColor());
			ttyTable.ttys[getCurrentTTY()].end++;
			offset++;
		}
	}
	offset = bckOffset;
}

void putsTTY( unsigned char *name, int count, tty_t tty )
{
	while( count-- >= 0 )
		putcharTTY(*name++, tty);
}

void putcharTTY( char c, tty_t tty )
{	
	int parse;
	
	if( offset == VIDEO_MEMORY_SIZE - 1 )
	{
		offset = 0;
		ttyTable.ttys[tty].begin = ttyTable.ttys[getCurrentTTY()].TerminalBuffer;
		ttyTable.ttys[tty].hasScrolled++;
	}
	parse = parseCharTTY(c, tty);
	if( parse )
	{
		++ttyTable.ttys[getCurrentTTY()].begin;
		++offset;
	}
}

void initializeTTY( void )
{
	int i;
	
	for( i = 0 ; i < MAX_TTY ; ++i )
	{
		ttyTable.ttys[i].ttyId = i;
		ttyTable.ttys[i].stdIn = stdinTableTTY[i].stdIN;
		ttyTable.ttys[i].hasScrolled = 0;
		stdinTableTTY[i].writeOffset = 0;
		stdinTableTTY[i].readOffset = 0;
		stdinTableTTY[i].empty = TRUE;
		putsTTY((unsigned char *) promnt, 17, i);
	}
	ttyTable.qtyTTY = MAX_TTY;
 	ttyTable.focusTTY= 0;
	ttyTable.ttys[0].focusProcess = 0;		/* Cuando arranca la TTY tiene al Idle corriendo */	
}

int getCurrentTTY( void ) 
{
	return getCurrentTTY();
}

void setTtyFocusProcess( tty_t tty, pid_t pid ){
	
		ttyTable.ttys[tty].focusProcess = pid;
}

int changeTtyFocus( tty_t nextTty ){	
	
	if( nextTty == getCurrentTTY() )
		return 1;
	ttyTable.focusTTY = ttyTable.ttys[nextTty].ttyId;
	refreshTTYScreen();
	return 0;
}

void putTTY(Keycode c, tty_t tty){
	
	int aux = stdinTableTTY[tty].writeOffset;
	
	if(stdinTableTTY[tty].empty == TRUE ){
		if(aux == MAX_LINE){
			aux = 0;
		}
		stdinTableTTY[tty].stdIN[aux++] = c;
		if(aux == stdinTableTTY[tty].readOffset){
			stdinTableTTY[tty].empty = FALSE;
		}
		
	}
	
}

static void refreshTTYKeyboardBuffer( void )
{
	Keycode deChar = 0;
	int color;
	
		color = getVideoColor();

		while( !kbBufferIsEmpty() )
			if( (deChar = charDeque()) != '\0' )
			{
				if( (deChar & 0x80 ) == 1 ){
					changeTtyFocus( (deChar & 0x81) - 1 );
				}else{
					if(runningProcess->ttyMode  == TTY_CANONICAL){
						putcharTTY((int)deChar, getCurrentTTY());
					}else{
						putTTY((int)deChar, getCurrentTTY());
					}
				}
			}

		
}

void refreshTTY(void){
	refreshTTYKeyboardBuffer();
	refreshTTYScreen();
}

Keycode getCharTTY(tty_t tty){
	Keycode aux ;
	
	do{
		/* TENGO QUE DORMIR EL PROCESO */
	}while(sleepCondition[tty] > 0);

	if(stdinTableTTY[tty].readOffset == MAX_LINE){
		stdinTableTTY[tty].readOffset = 0;
	}
	
	aux = stdinTableTTY[tty].stdIN[stdinTableTTY[tty].readOffset++];
	
	if(aux == NEW_LINE){
		sleepCondition[tty] --;
	}
	
	return  aux;

}


	
