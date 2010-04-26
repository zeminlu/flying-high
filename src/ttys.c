/*
 * ttys.c
 */

#include "ttys.h"

static int offset = 0;

typedef struct TTY{
	tty_t	ttyId;
	Keycode *	stdIn;
	unsigned char TerminalBuffer[VIDEO_MEMORY_SIZE];
	unsigned char 	*begin, *end;
	int		hasScrolled;
	pid_t	focusProcess;
}TTY;

typedef struct sysTTY{
	int		qtyTTY;
	TTY		ttys[MAX_TTY];
	tty_t	focusTTY;
}sysTTY;

typedef void (*printFunctions)(unsigned char * p);

sysTTY ttyTable;

static int sleepCondition[MAX_TTY]= {FALSE};

/*
 *	Functions Section
 */

/*
 *	Static functions for refreshTTY
 */

static void moveAlarmTTY( unsigned char * p )
{
	return;	
}

static void moveBackSpaceTTY( unsigned char * p )
{	
	return;
}

static void moveTabTTY(unsigned char * p )
{
	putCharAtCurrentPos( TAB, getVideoColor());
	p += TAB;
	offset += TAB;	
}

static void moveNewLineTTY( unsigned char * p )
{
	putCharAtCurrentPos( NEW_LINE, getVideoColor());
	do{
		++p;
		++offset;
	}while( (offset % SCREEN_WIDTH) != 0 );
}

static void moveVTabTTY( unsigned char * p )
{
	int i, curOffset;

	putCharAtCurrentPos( VTAB, getVideoColor());
	curOffset = offset % SCREEN_WIDTH;
	for( i = 0 ; i < VIDEO_VTAB_STOP ; ++i )
	{
		moveNewLineTTY(p);
	}
	while( curOffset >= offset)
	{
		++offset;
		++p;
	}
}

static void moveLineFeedTTY( unsigned char * p )
{
	int curOffset;

	moveNewLineTTY(p);
	curOffset = offset % SCREEN_WIDTH;
	do
	{
		++p;
		++offset;
	}while( offset == curOffset );
}

static void moveReturnTTY( unsigned char * p )
{
	while( (offset % SCREEN_WIDTH) != 0 )
	{
		--p;
		--offset;
	}
}

/*
 *	Static functions for putcharTTY
 */

static void parseAlarmTTY( unsigned char * p )
{
	return;	
}

static void parseBackSpaceTTY( unsigned char * p )
{
	if( offset > 0 )
	{
		--p;
		*p++ = ' ';
	}
}

static void parseTabTTY(unsigned char * p )
{
	*p = TAB;
	p += VIDEO_TAB_STOP;
	offset += VIDEO_TAB_STOP;	
}

static void parseNewLineTTY( unsigned char * p )
{
	*p = NEW_LINE;
	do{
		++p;
		++offset;
	}while( (offset % SCREEN_WIDTH) != 0 );
}

static void parseVTabTTY( unsigned char * p )
{
	int i, curOffset;
	
	*p = VTAB;
	curOffset = offset % SCREEN_WIDTH;
	for( i = 0 ; i < VIDEO_VTAB_STOP ; ++i )
	{
		parseNewLineTTY(p);
	}
	while( curOffset >= offset)
	{
		++offset;
		++p;
	}
}

static void parseLineFeedTTY( unsigned char * p )
{
	int curOffset;
	
	parseNewLineTTY(p);
	curOffset = offset % SCREEN_WIDTH;
	do
	{
		++p;
		++offset;
	}while( offset == curOffset );
}

static void parseReturnTTY( unsigned char * p )
{
	while( (offset % SCREEN_WIDTH) != 0 )
	{
		--p;
		--offset;
	}
}

static int parseCharTTY( int c )
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
		specialCharPrint[c - '\a'](ttyTable.ttys[ttyTable.focusTTY].begin);
		return 0;
	}else
	{
		*(ttyTable.ttys[ttyTable.focusTTY].begin) = c;
		return 1;
	}
}

static void refreshTTYScreen( void )
{
	int bckOffset, character;
	static printFunctions moveCharPrint[] = {
		moveAlarmTTY,
		moveBackSpaceTTY,
		moveTabTTY,
		moveNewLineTTY,
		moveVTabTTY,
		moveLineFeedTTY,
		moveReturnTTY
	};
	
	bckOffset = offset;
	offset = 0;
	
	if( ttyTable.ttys[ttyTable.focusTTY].hasScrolled > 0 )
	{
		ttyTable.ttys[ttyTable.focusTTY].end = ttyTable.ttys[ttyTable.focusTTY].begin + (bckOffset % SCREEN_WIDTH) + 1;
		offset = (ttyTable.ttys[ttyTable.focusTTY].begin - ttyTable.ttys[ttyTable.focusTTY].TerminalBuffer ) + SCREEN_WIDTH;
	}
	while( ttyTable.ttys[ttyTable.focusTTY].end != ttyTable.ttys[ttyTable.focusTTY].begin )
	{
		if( offset == VIDEO_MEMORY_SIZE )
			offset = 0;
		character = *(ttyTable.ttys[ttyTable.focusTTY].end);
		if( '\a' <= character && character >= '\r' )
		{
			moveCharPrint[character - '\a'](ttyTable.ttys[ttyTable.focusTTY].end);
		}else
		{
			putCharAtCurrentPos((int)(*ttyTable.ttys[ttyTable.focusTTY].end), getVideoColor());
			ttyTable.ttys[ttyTable.focusTTY].end++;
			offset++;
		}
	}
	offset = bckOffset;
}

void putsTTY( unsigned char *name, int count )
{
	while( count-- >= 0 )
		putcharTTY(*name++);
}

void putcharTTY( char c )
{	
	int parse;
	
	if( offset == VIDEO_MEMORY_SIZE - 1 )
	{
		offset = 0;
		ttyTable.ttys[ttyTable.focusTTY].begin = ttyTable.ttys[ttyTable.focusTTY].TerminalBuffer;
		ttyTable.ttys[ttyTable.focusTTY].hasScrolled++;
	}
	parse = parseCharTTY(c);
	if( parse )
	{
		++ttyTable.ttys[ttyTable.focusTTY].begin;
		++offset;
	}
}

void initializeTTY( void )
{
	int i;
	
	for( i = 0 ; i < MAX_TTY ; ++i )
	{
		memcpy(ttyTable.ttys[i].TerminalBuffer, "root@ArkOS$>", 13 );
		ttyTable.ttys[i].ttyId = i;
		ttyTable.ttys[i].stdIn = keyboardBuffer;
		ttyTable.ttys[i].hasScrolled = 0;
	}
	ttyTable.qtyTTY = MAX_TTY;
	ttyTable.focusTTY = 0;
	ttyTable.ttys[0].focusProcess = 0;		/* Cuando arranca la TTY tiene al Idle corriendo */	
}

int getCurrentTTY( void ) 
{
	return ttyTable.focusTTY;
}

void setTtyFocusProcess( tty_t tty, pid_t pid )
{
		ttyTable.ttys[tty].focusProcess = pid;
}

int changeTtyFocus( tty_t nextTty )
{
	
	if( nextTty == ttyTable.focusTTY )
		return 1;
	ttyTable.focusTTY = ttyTable.ttys[nextTty].ttyId;
	refreshTTYScreen();	
	return 0;
}


static void SysWriteTTY(tty_t ttyId, char * buffer, int amm){

	/*Funcion que agrega a la queue de video*/
	if(ttyTable.focusTTY == ttyId){
		/*Quiero escribir en mi buffer y en pantalla*/
		printToScreen(buffer, amm);
	
	}
}

void printToScreen(char * str, int amm){
	int i; 
	for(i = 0; i < amm ; i++){
		putCharAtCurrentPos((int)str[i], getVideoColor());
	}
}

int SysReadTTY(tty_t ttyId){
	/*hay algo disponible para leer en stdin*/
	if( !kbBufferIsEmpty() ){
		/*si lo hay lo retorno en buffer*/
		return(int)charDeque();
	}else{
		while(!sleepCondition[ttyTable.focusTTY]){
			waitTty(ttyTable.focusTTY);
		}
		return(int)charDeque();
	}
	return 0;
}


	
