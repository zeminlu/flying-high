/*
 * ttys.c
 */

#include "ttys.h"

static short cursorOffset = 0;

static int cursorRow = CURSOR_START_ROW;

static int cursorCol = CURSOR_START_COL;
	
typedef void (*printFunctions)(unsigned char *, int, tty_t);

sysTTY ttyTable;

static stdInTTY stdinTableTTY[MAX_TTY];

int sleepCondition[MAX_TTY]= {0};

/*
 *	Static functions for putcharTTY
 */

static void incTTYCursor ( void )
{
	cursorOffset++;
	++cursorCol;
	if ( cursorCol == SCREEN_WIDTH )
	{
		cursorCol = 0;
		++cursorRow;
	}
	if ( cursorRow == SCREEN_HEIGTH )
		cursorRow = 0;
}

static void decTTYCursor ( void )
{
	if ( (cursorOffset--) < 0 )
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

static void putLine( tty_t tty)
{

	if(write(STDIN , (char *)stdinTableTTY[tty].stdIN, 5 ) == 5){
		/*sleepCondition[tty] += stdinTableTTY[tty].writeOffset;*/
		signalTty(tty);
		
	}
//	_debug();
}

static void parseAlarmTTY( unsigned char * p, int where, tty_t tty )
{
	printAlarm();
}

static void parseBackSpaceTTY( unsigned char * p, int where, tty_t tty )
{
	static int cond = FALSE;
	int i;
	
	if( !where )
	{
		cond = FALSE;
		if (p[cursorOffset] == TAB){
			cursorOffset -= VIDEO_TAB_STOP;
			cond = TRUE;
		}
		else
			--cursorOffset;
		p[cursorOffset] = ' ';
	}
	else{
		if (cond){
			for (i = 0 ; i < VIDEO_TAB_STOP ; ++i)
				printBackspace ();
			cond = FALSE;
		}
		else
			printBackspace ();
	}
	return;
}

static void parseTabTTY(unsigned char * p, int where, tty_t tty )
{	
	if( !where ){
		cursorOffset += VIDEO_TAB_STOP;
		p[cursorOffset] = TAB;
	}
	else 
		printTab();
}

static void parseNewLineTTY( unsigned char * p, int where, tty_t tty )
{
	if( !where ) {
		p[cursorOffset] = NEW_LINE;
		cursorOffset += SCREEN_WIDTH - (cursorOffset % SCREEN_WIDTH);
	}
	else
		printNewLine();
}

static void parseVTabTTY( unsigned char * p, int where, tty_t tty )
{
	int i, curOffset;
	
	if( !where ) 
		p[cursorOffset] = VTAB;
	else 
		printVTab();
	curOffset = cursorOffset % SCREEN_WIDTH;
	for( i = 0 ; i < VIDEO_VTAB_STOP ; ++i )
	{
		parseNewLineTTY(p, where, tty);
	}
	while( curOffset >= cursorOffset)
	{
		incTTYCursor();
	}
}

static void parseLineFeedTTY( unsigned char * p, int where, tty_t tty )
{
	int curOffset;
	
	if( !where )
		parseNewLineTTY(p,where, tty);
	else
		printNewLine();
	curOffset = cursorOffset % SCREEN_WIDTH;
	do
	{
		incTTYCursor();
	}while( curOffset-- >= 0 );
}

static void parseReturnTTY( unsigned char * p, int where, tty_t tty )
{
	printReturn();
	while( (cursorOffset % SCREEN_WIDTH) != 0 )
	{
		decTTYCursor();
	}
}

static int parseCharTTY( int c, tty_t tty, int inStdIn)
{
	int bckOffset;
	static printFunctions specialCharPrint[] = {
		parseAlarmTTY,
		parseBackSpaceTTY,
		parseTabTTY,
		parseNewLineTTY,
		parseVTabTTY,
		parseLineFeedTTY,
		parseReturnTTY
	};
	if( '\a' <= c && c <= '\r' )
	{
		bckOffset = cursorOffset;
		specialCharPrint[c - '\a'](ttyTable.ttys[tty].begin, WRITE_ON_TTY, tty);
		if (c == '\n' && inStdIn)
			putLine( tty );
		cursorOffset = bckOffset;
		if( tty == getFocusTTY() )
			specialCharPrint[c - '\a'](ttyTable.ttys[tty].begin, SEND_TO_VIDEO, tty);
		return 0;
	}else
	{
		((ttyTable.ttys[getFocusTTY()]).begin++)[cursorOffset] = c;
		if( tty == getFocusTTY() )
			printChar( c );
		return 1;
	}
}

static void refreshScreenTTY( void )
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
	
	bckOffset = cursorOffset;
	cursorOffset = 0;
	
	setCursorPosition(0,0);
	if( ttyTable.ttys[getFocusTTY()].hasScrolled > 0 )
	{
		ttyTable.ttys[getFocusTTY()].end = ttyTable.ttys[getFocusTTY()].begin + (SCREEN_WIDTH - (bckOffset % SCREEN_WIDTH));
		cursorOffset = bckOffset + (SCREEN_WIDTH - (bckOffset % SCREEN_WIDTH));
	}
	while( ttyTable.ttys[getFocusTTY()].end != (ttyTable.ttys[getFocusTTY()].begin + bckOffset) )
	{
		if( cursorOffset == VIDEO_MEMORY_SIZE )
			cursorOffset = 0;
		character = (ttyTable.ttys[getFocusTTY()].end)[cursorOffset];
		if( '\a' <= character && character >= '\r' )
		{
			refreshCharPrint[character - '\a'](ttyTable.ttys[getFocusTTY()].end, SEND_TO_VIDEO, getFocusTTY());
		}else
		{
			printChar( (ttyTable.ttys[getFocusTTY()].end)[cursorOffset] );
			incTTYCursor();
		}
	}
	cursorOffset = bckOffset;
}

void putsTTY( unsigned char *name, int count, tty_t tty )
{
	while( count-- >= 0 )
		putCharTTY(*name++, tty, TRUE);
}

void putCharTTY( char c, tty_t tty, int inStdIn )
{	
	int parse;
	
	if( cursorOffset == VIDEO_MEMORY_SIZE )
	{
		cursorOffset = 0;
		ttyTable.ttys[tty].begin = ttyTable.ttys[tty].TerminalBuffer;
		ttyTable.ttys[tty].hasScrolled++;
	}
	parse = parseCharTTY(c, tty, inStdIn);
	if( parse )
	{
		++cursorOffset;
	}
}

void initializeTTY( void )
{
	int i;
	for( i = 0 ; i < MAX_TTY ; ++i )
	{
		ttyTable.ttys[i].ttyId = i;
		ttyTable.ttys[i].inTTY = stdinTableTTY[i].stdIN;
		ttyTable.ttys[i].hasScrolled = 0;
		stdinTableTTY[i].writeOffset = 0;
		stdinTableTTY[i].readOffset = 0;
		stdinTableTTY[i].empty = TRUE;
	}

	ttyTable.qtyTTY = MAX_TTY;
 	ttyTable.focusTTY= STD_TTY;
	ttyTable.ttys[STD_TTY].focusProcess = 0;		/* Cuando arranca la TTY tiene al Idle corriendo */	
}

int getFocusTTY( void ) 
{
	return ttyTable.focusTTY;
}

void setFocusProcessTTY( tty_t tty, pid_t pid ){
	
	ttyTable.ttys[tty].focusProcess = pid;
}
	
void putTTY(Keycode c, tty_t tty){
/*
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
*/	
	if(write( STDIN , (char *)&c, 1 ) == 1){
		sleepCondition[tty]++;
		signalTty(tty);
	}	
}

static void refreshKeyboardBufferTTY( void ){
	Keycode deChar = 0;
	int color;
	
		color = getVideoColor();
	
		while( !kbBufferIsEmpty() ){
			if( (deChar = charDeque()) != '\0' )
			{
				if( 0x81 <= deChar && deChar <= 0x88  ){
					changeFocusTTY( (deChar - 0x81) );
				}else{
					if(runningProcess->ttyMode  == TTY_CANONICAL){
						putCharTTY((int)deChar, getFocusTTY(), TRUE);
						
					}else{
						putTTY((int)deChar, getFocusTTY());
					}
				}
			}
		}
}

int changeFocusTTY( tty_t nextTty ){	
	
	if( nextTty == getFocusTTY() )
		return 1;
	clearScreen();
	ttyTable.focusTTY = ttyTable.ttys[nextTty].ttyId;
	setCursorPosition(0,0);
//	refreshScreenTTY();
//	refreshScreen();
	return 0;
}

void refreshStdout(void){
	char aux;

	while(read(STDOUT, &aux, 1 ) == 1){
		putCharTTY(aux,  runningProcess->tty, FALSE);
	}

}

void refreshTTY(void){

	if(runningProcess != NULL && runningProcess != initProcess && runningProcess->pid > 0){
		refreshStdout();
		//if(runningProcess->tty == getFocusTTY()){
			refreshKeyboardBufferTTY();
			refreshScreen();
		//}	
	}
}

/*
 *Public Functions
 */

void sysSetFocusProcessTTY(pid_t pid, tty_t tty){
	ttyTable.ttys[tty].focusProcess = pid; 
}

pid_t sysGetFocusProcessTTY(tty_t tty){
	return ttyTable.ttys[tty].focusProcess;
}
/*
Keycode sysGetChar(tty_t tty){
	Keycode aux ;
	
	while(sleepCondition[tty] <= 0){
		waitTty(tty);
	}

	if(stdinTableTTY[tty].readOffset == MAX_LINE){
		stdinTableTTY[tty].readOffset = 0;
	}
	
	aux = stdinTableTTY[tty].stdIN[stdinTableTTY[tty].readOffset++];
	
	if(runningProcess->ttyMode == TTY_CANONICAL && aux == NEW_LINE){
		sleepCondition[tty] --;
	}else if(runningProcess->ttyMode != TTY_CANONICAL){
		sleepCondition[tty] --;
	}
	
	return  aux;
}
*/

