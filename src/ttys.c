/*
 * ttys.c
 */

#include "ttys.h"

static short writePointer = 0;

static int writeRow = CURSOR_START_ROW;

static int writeCol = CURSOR_START_COL;

static short readPointer = 0;

static int readRow = CURSOR_START_ROW;

static int readCol = CURSOR_START_COL;

typedef void (*videoParser)( void );

typedef void (*printFunctions)( int, tty_t);

sysTTY ttyTable;

static stdInTTY stdinTableTTY[MAX_TTY];

int sleepCondition[MAX_TTY]= {0};

static videoParser videoParserFunctions[] = {
printAlarm,
	printBackspace,
	printTab,
	printNewLine,
	printVTab,
	printLineFeed,
	printReturn
};

/*
*	Static functions for putcharTTY
*/
static void loadPointer( tty_t tty, int what ){
	
	if( what == SAVE ){
		ttyTable.listTTY[tty]->writePointer = writePointer;
		ttyTable.listTTY[tty]->writeCol = writeCol;
		ttyTable.listTTY[tty]->writeRow = writeRow;
	}else{
		writePointer = ttyTable.listTTY[tty]->writePointer;
		writeCol = ttyTable.listTTY[tty]->writeCol;
		writeRow = ttyTable.listTTY[tty]->writeRow;
	}
}


static void incReadOffset ( void )
{
	readPointer++;
	++readCol;
	if ( readCol == SCREEN_WIDTH )
	{
		readCol = 0;
		++readRow;
	}
	if ( readRow == SCREEN_HEIGTH )
		readRow = 0;
}

static void decReadOffset ( void )
{
	if ( (readPointer--) < 0 )
		readPointer = 0;

	--readCol;
	if ( readCol == -1 )
	{
		readCol = SCREEN_WIDTH - 1;
		--readRow;
	}
	if ( readRow == -1 )
	{
		readRow = 0;
		readCol = 0;
	}
}

static void incwritePointer ( void )
{
	writePointer++;
	++writeCol;
	if ( writeCol == SCREEN_WIDTH )
	{
		writeCol = 0;
		++writeRow;
	}
	if ( writeRow == SCREEN_HEIGTH )
		writeRow = 0;
}

static void decwritePointer ( void )
{
	if ( (writePointer--) < 0 )
		writePointer = 0;

	--writeCol;
	if ( writeCol == -1 )
	{
		writeCol = SCREEN_WIDTH - 1;
		--writeRow;
	}
	if ( writeRow == -1 )
	{
		writeRow = 0;
		writeCol = 0;
	}
}

static void putLine( tty_t tty)
{
	write(STDIN, (char *)(ttyTable.listTTY[tty]->stdIn->stdInBuffer), ttyTable.listTTY[tty]->stdIn->writeOffset);
	signalTty(tty);
	ttyTable.listTTY[tty]->stdIn->writeOffset = 0;
}

static void parseAlarmTTY( int where, tty_t tty )
{
}


static void parseBackSpaceTTY( int where, tty_t tty )
{
	static int cond = FALSE;
	int i;
	
	if( !where )
	{
		cond = FALSE;
		if ((ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] == TAB){
			writePointer -= VIDEO_TAB_STOP;
			cond = TRUE;
		}
		else
			decwritePointer();
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] = ' ';
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

static void parseTabTTY( int where, tty_t tty )
{	
	int i, prevRow;
	
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] = TAB;
		for ( i = 0, prevRow = writeRow; i < VIDEO_TAB_STOP; ++i )
		{
			if ( prevRow != writeRow )
				break;
			(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] = ' ';
			incwritePointer();
		}
	}
	else{
		for ( i = 0, prevRow = readRow; i < VIDEO_TAB_STOP; ++i )
		{
			if ( prevRow != readRow )
				break;
			(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[readPointer] = ' ';
			incReadOffset();
		}
	}
}

static void parseNewLineTTY( int where, tty_t tty )
{
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] = NEW_LINE;
		do
		{
			incwritePointer();
		} while ( writeCol != 0 );
	}
	else{
		do
		{
			incReadOffset();
		} while ( readCol != 0 );
	}
}

static void parseVTabTTY( int where, tty_t tty )
{
	int i, col;
	
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] = VTAB;
		col = writeCol - 1;
		for ( i = 0; i < VIDEO_VTAB_STOP; ++i )
			parseNewLineTTY( where, tty);
		while ( writeCol < col )
			incwritePointer();
	} 
	else{
		printVTab();
		col = readCol - 1;
		for ( i = 0; i < VIDEO_VTAB_STOP; ++i )
			parseNewLineTTY( where, tty);
		while ( readCol < col )
			incReadOffset();
	}
}

static void parseLineFeedTTY( int where, tty_t tty )
{
	int col;
	
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] = LINE_FEED;
		parseNewLineTTY(where, tty);
		col = writeCol;
		do
		{
			incwritePointer();
		} while ( writeCol == col );
	}
	else{
		printLineFeed();
		col = readCol;
		do
		{
			incReadOffset();
		} while ( readCol == col );
	}
}

static void parseReturnTTY( int where, tty_t tty )
{
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[writePointer] = RETURN;
		while ( writeCol != 0 )
			decwritePointer();
	}else{
		printReturn();
		while ( readCol != 0 )
			decReadOffset();
	}
}

static int parseCharTTY( int c, tty_t tty, int inStdIn)
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

	if( '\a' <= c && c <= '\r' )
	{
		specialCharPrint[c - '\a']( WRITE_ON_TTY, tty);
		if (c == '\n' && tty == getFocusTTY() && inStdIn)
			putLine( tty );
		if( tty == getFocusTTY() )
			videoParserFunctions[c - '\a']();
		return 0;
	}else
	{
		((ttyTable.listTTY[tty])->stdOut->stdOutBuffer)[writePointer] = c;
		if( inStdIn && getFocusTTY() == tty )
			ttyTable.listTTY[tty]->stdIn->stdInBuffer[(ttyTable.listTTY[tty]->stdIn->writeOffset)++]= c;
		if( tty == getFocusTTY() )
			printChar(c);
		return 1;
	}
}

static void refreshScreenTTY( void )
{
	int character;
	static printFunctions refreshCharPrint[] = {
		parseAlarmTTY,
		parseBackSpaceTTY,
		parseTabTTY,
		parseNewLineTTY,
		parseVTabTTY,
		parseLineFeedTTY,
		parseReturnTTY
	};
	
	readPointer = readCol = readRow = 0;
	if( ttyTable.listTTY[getFocusTTY()]->hasScrolled > 0 ){
		readPointer = writePointer + (SCREEN_WIDTH - (writePointer % SCREEN_WIDTH));
		readRow = writeRow + 1;
	}
	while( readPointer != writePointer ){
		if( readPointer == SCREEN_SIZE )
			readPointer = 0;
		character = (ttyTable.listTTY[getFocusTTY()]->stdOut->stdOutBuffer)[readPointer];
		if( '\a' <= character && character >= '\r' )
		{
			refreshCharPrint[character - '\a'](SEND_TO_VIDEO, getFocusTTY());
		}else
		{
			printChar((ttyTable.listTTY[getFocusTTY()]->stdOut->stdOutBuffer)[readPointer]);
			/*write( STDIN, &(ttyTable.ttys[getFocusTTY()].end)[readPointer], 1 );
			*/
			incReadOffset();
		}
	}
}

void putsTTY( unsigned char *name, int count, tty_t tty )
{
	while( count-- >= 0 )
		putCharTTY(*name++, tty, TRUE);
}

void putCharTTY( char c, tty_t tty, int inStdIn )
{	
	int parse;
	
	if( writePointer == VIDEO_MEMORY_SIZE )
	{
		writePointer = 0;
		ttyTable.listTTY[tty]->hasScrolled++;
	}
	parse = parseCharTTY(c, tty, inStdIn);
	if( parse )
	{
		++writePointer;
	}
}

void initializeTTY( void )
{
	int i;
	
	ttyTable.qtyTTY = MAX_TTY;
	ttyTable.listTTY = kMalloc(sizeof(tty_s *) * MAX_TTY);
	ttyTable.focusTTY = STD_TTY;
	for( i = 0 ; i < MAX_TTY ; ++i ){
		ttyTable.listTTY[i] = kMalloc(sizeof(tty_s));
		ttyTable.listTTY[i]->ttyId = i;
		ttyTable.listTTY[i]->stdIn = kMalloc(sizeof(stdInTTY));
		ttyTable.listTTY[i]->stdIn->stdInBuffer = kMalloc(sizeof(Keycode) * MAX_LINE);
		ttyTable.listTTY[i]->stdIn->writeOffset = 0;
		ttyTable.listTTY[i]->stdOut = kMalloc(sizeof(stdOutTTY));
		ttyTable.listTTY[i]->stdOut->stdOutBuffer = kMalloc(sizeof(Keycode) * VIDEO_MEMORY_SIZE);
		ttyTable.listTTY[i]->hasScrolled = 0;
		ttyTable.listTTY[i]->focusProcess  = 0;
		ttyTable.listTTY[i]->stdOut->begin = ttyTable.listTTY[i]->stdOut->end = ttyTable.listTTY[i]->stdOut->stdOutBuffer;
		ttyTable.listTTY[i]->writePointer = 0;
		ttyTable.listTTY[i]->writeCol = CURSOR_START_COL;
		ttyTable.listTTY[i]->writeRow = CURSOR_START_ROW;
	}
}

int getFocusTTY( void ) 
{
	return ttyTable.focusTTY;
}

void setFocusProcessTTY( tty_t tty, pid_t pid ){
	
	(ttyTable.listTTY[tty])->focusProcess = pid;
}
	
void putTTY(Keycode c, tty_t tty){
/*
	int aux = stdinTableTTY[tty].writePointer;
	
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
				break;
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
	
	/*char *p = kMalloc( sizeof(char) * 10);
	*/
	if( nextTty == getFocusTTY() )
		return 1;
	clearScreen();
	ttyTable.focusTTY = ttyTable.listTTY[nextTty]->ttyId;
	setCursorPosition(0,0);
	
	/*puts(intToString(writePointer,p));
	puts(intToString(writeCol,p));
	puts(intToString(writeRow,p));
	puts(intToString(readPointer,p));
	puts(intToString(readCol,p));
	puts(intToString(readRow,p));
	*/
	loadPointer( getFocusTTY(), SAVE );
	loadPointer( nextTty, LOAD );
//	refreshScreenTTY();
	refreshScreen();
	return 0;
}

void refreshStdout(void){
	char aux;

	while(read(STDOUT, &aux, 1 ) == 1){
		putCharTTY(aux, runningProcess->tty, FALSE);
	}

}

void refreshTTY(void){

	if(runningProcess != NULL && runningProcess != initProcess && runningProcess->pid > 0 && runningProcess->tty != -1){
		refreshStdout();
		refreshKeyboardBufferTTY();
		refreshScreen();
		//if (runningProcess->tty == getFocusTTY()){
		//}
	}
	
	return;
}

/*
 *Public Functions
 */

void sysSetFocusProcessTTY(pid_t pid, tty_t tty){
	ttyTable.listTTY[tty]->focusProcess = pid; 
}

pid_t sysGetFocusProcessTTY(tty_t tty){
	return ttyTable.listTTY[tty]->focusProcess;
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

