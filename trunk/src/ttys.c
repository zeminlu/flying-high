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

typedef void (*printFunctions)(unsigned char *, int, tty_t);

sysTTY ttyTable;

static int sleepCondition[MAX_TTY]= {FALSE};

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
			while( *(ttyTable.ttys[tty].stdIn) != NEW_LINE )
			{
				putCharAtCurrentPos( *(ttyTable.ttys[tty].stdIn), getVideoColor() );
				(ttyTable.ttys[tty].stdIn)++;
			}
			break;
		case TTY_CANONICAL:
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
		if( offset > 0 )
		{
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
	do{
		++p;
		++offset;
	}while( (offset % SCREEN_WIDTH) != 0 );
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
		specialCharPrint[c - '\a'](ttyTable.ttys[ttyTable.focusTTY].begin, WRITE_ON_TTY, tty);
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
			refreshCharPrint[character - '\a'](ttyTable.ttys[ttyTable.focusTTY].end, SEND_TO_VIDEO, ttyTable.focusTTY);
		}else
		{
			putCharAtCurrentPos((int)(*(ttyTable.ttys[ttyTable.focusTTY].end)), getVideoColor());
			ttyTable.ttys[ttyTable.focusTTY].end++;
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
		ttyTable.ttys[tty].begin = ttyTable.ttys[ttyTable.focusTTY].TerminalBuffer;
		ttyTable.ttys[tty].hasScrolled++;
	}
	parse = parseCharTTY(c, tty);
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


	
