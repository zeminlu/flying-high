/*
 * ttys.c
 */

#include "ttys.h"

typedef void (*videoParser)( void );

typedef void (*printFunctions)( int, tty_t);

sysTTY ttyTable;

static stdInTTY stdinTableTTY[MAX_TTY];

int sleepCondition[MAX_TTY]= {0};

tty_s TTYsList[MAX_TTY];

stdInTTY TTYstdIns[MAX_TTY];

stdOutTTY TTYstdOuts[MAX_TTY];

Keycode stdInBuffers[MAX_TTY][MAX_LINE];

Keycode stdOutBuffers[MAX_TTY][VIDEO_MEMORY_SIZE];

static videoParser videoParserFunctions[] = {
	printAlarm,
	printBackspace,
	printTab,
	printNewLine,
	printVTab,
	printLineFeed,
	printReturn
};


void initializeTTY( void )
{
	int i;
	
	ttyTable.qtyTTY = MAX_TTY;
	ttyTable.focusTTY = STD_TTY;
	for( i = 0 ; i < MAX_TTY ; ++i ){
		ttyTable.listTTY[i] = &(TTYsList[i]);
		ttyTable.listTTY[i]->ttyId = i;
		ttyTable.listTTY[i]->stdIn = &(TTYstdIns[i]);
		ttyTable.listTTY[i]->stdIn->stdInBuffer = stdInBuffers[i];
		ttyTable.listTTY[i]->stdIn->writeOffset = 0;
		ttyTable.listTTY[i]->stdOut = &(TTYstdOuts[i]);
		ttyTable.listTTY[i]->stdOut->stdOutBuffer = stdOutBuffers[i];
		ttyTable.listTTY[i]->hasScrolled = 0;
		ttyTable.listTTY[i]->focusProcess  = 0;
		ttyTable.listTTY[i]->stdOut->begin = ttyTable.listTTY[i]->stdOut->end = ttyTable.listTTY[i]->stdOut->stdOutBuffer;
		ttyTable.listTTY[i]->writePointer = 0;
		ttyTable.listTTY[i]->writeCol = CURSOR_START_COL;
		ttyTable.listTTY[i]->writeRow = CURSOR_START_ROW;
		ttyTable.listTTY[i]->readPointer = 0;
		ttyTable.listTTY[i]->readCol = CURSOR_START_COL;
		ttyTable.listTTY[i]->readRow = CURSOR_START_ROW;
	}
}

/*
*	Static functions for putcharTTY
*/

static void incReadPointer ( int *readPointer, int *readCol, int *readRow )
{
	(*readPointer)++;
	++(*readCol);
	if ( (*readCol) == SCREEN_WIDTH )
	{
		(*readCol) = 0;
		++(*readRow);
	}
	if ( (*readRow) == SCREEN_HEIGTH )
		(*readRow) = 0;
}

static void decReadPointer ( int *readPointer, int *readCol, int *readRow )
{
	if ( ((*readPointer)--) < 0 )
		(*readPointer) = 0;

	--(*readCol);
	if ( (*readCol) == -1 )
	{
		(*readCol) = SCREEN_WIDTH - 1;
		--(*readRow);
	}
	if ( (*readRow) == -1 )
	{
		(*readRow) = 0;
		(*readCol) = 0;
	}
}

static void incWritePointer ( int *writePointer, int *writeCol, int *writeRow )
{
	(*writePointer)++;
	++(*writeCol);
	if ( (*writeCol) == SCREEN_WIDTH )
	{
		(*writeCol) = 0;
		++(*writeRow);
	}
	if ( (*writeRow) == SCREEN_HEIGTH )
		(*writeRow) = 0;
}

static void decWritePointer ( int *writePointer, int *writeCol, int *writeRow )
{
	if ( ( (*writePointer)--) < 0 )
		(*writePointer) = 0;

	--(*writeCol);
	if ( (*writeCol) == -1 )
	{
		(*writeCol) = SCREEN_WIDTH - 1;
		--(*writeRow);
	}
	if ( (*writeRow) == -1 )
	{
		(*writeRow) = 0;
		(*writeCol) = 0;
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
		if ((ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] == TAB){
			ttyTable.listTTY[tty]->writePointer -= VIDEO_TAB_STOP;
			cond = TRUE;
		}
		else
			decWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = ' ';
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
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = TAB;
		for ( i = 0, prevRow = ttyTable.listTTY[tty]->writeRow; i < VIDEO_TAB_STOP; ++i )
		{
			if ( prevRow != ttyTable.listTTY[tty]->writeRow )
				break;
			(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = ' ';
			incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );		
		}
	}
	else{
		for ( i = 0, prevRow = ttyTable.listTTY[tty]->readRow; i < VIDEO_TAB_STOP; ++i )
		{
			if ( prevRow != ttyTable.listTTY[tty]->readRow )
				break;
			(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->readPointer] = ' ';
			incReadPointer( &(ttyTable.listTTY[tty]->readPointer), &(ttyTable.listTTY[tty]->readCol), &(ttyTable.listTTY[tty]->readRow));
		}
	}
}

static void parseNewLineTTY( int where, tty_t tty )
{
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = NEW_LINE;
		do
		{
			incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );
		} while ( ttyTable.listTTY[tty]->writeCol != 0 );
	}
	else{
		printNewLine();
		do
		{
			incReadPointer( &(ttyTable.listTTY[tty]->readPointer), &(ttyTable.listTTY[tty]->readCol), &(ttyTable.listTTY[tty]->readRow) );
		} while ( ttyTable.listTTY[tty]->readCol != 0 );
	}
}

static void parseVTabTTY( int where, tty_t tty )
{
	int i, col;
	
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = VTAB;
		col = ttyTable.listTTY[tty]->writeCol - 1;
		for ( i = 0; i < VIDEO_VTAB_STOP; ++i )
			parseNewLineTTY( where, tty);
		while ( ttyTable.listTTY[tty]->writeCol < col )
			incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );
	} 
	else{
		printVTab();
		col = ttyTable.listTTY[tty]->readCol - 1;
		for ( i = 0; i < VIDEO_VTAB_STOP; ++i )
			parseNewLineTTY( where, tty);
		while ( ttyTable.listTTY[tty]->readCol < col )
			incReadPointer( &(ttyTable.listTTY[tty]->readPointer), &(ttyTable.listTTY[tty]->readCol), &(ttyTable.listTTY[tty]->readRow) );
	}
}

static void parseLineFeedTTY( int where, tty_t tty )
{
	int col;
	
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = LINE_FEED;
		parseNewLineTTY(where, tty);
		col = ttyTable.listTTY[tty]->writeCol;
		do
		{
			incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );
		} while ( ttyTable.listTTY[tty]->writeCol == col );
	}
	else{
		printLineFeed();
		col = ttyTable.listTTY[tty]->readCol;
		do
		{
			incReadPointer( &(ttyTable.listTTY[tty]->readPointer), &(ttyTable.listTTY[tty]->readCol), &(ttyTable.listTTY[tty]->readRow) );
		} while ( ttyTable.listTTY[tty]->readCol == col );
	}
}

static void parseReturnTTY( int where, tty_t tty )
{
	if( !where ){
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = RETURN;
		while ( ttyTable.listTTY[tty]->writeCol != 0 )
			decWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );
	}else{
		printReturn();
		while ( ttyTable.listTTY[tty]->readCol != 0 )
			decReadPointer( &(ttyTable.listTTY[tty]->readPointer), &(ttyTable.listTTY[tty]->readCol), &(ttyTable.listTTY[tty]->readRow));
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
		((ttyTable.listTTY[tty])->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = c;
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
	
	ttyTable.listTTY[getFocusTTY()]->readPointer = ttyTable.listTTY[getFocusTTY()]->readCol = ttyTable.listTTY[getFocusTTY()]->readRow = 0;
	if( ttyTable.listTTY[getFocusTTY()]->hasScrolled > 0 ){
		
		ttyTable.listTTY[getFocusTTY()]->readPointer = ttyTable.listTTY[getFocusTTY()]->writePointer;
		do{
			incReadPointer( &(ttyTable.listTTY[getFocusTTY()]->readPointer), &(ttyTable.listTTY[getFocusTTY()]->readCol), &(ttyTable.listTTY[getFocusTTY()]->readRow));
		}while( ttyTable.listTTY[getFocusTTY()]->readCol != 0 );
		
		ttyTable.listTTY[getFocusTTY()]->readRow = ttyTable.listTTY[getFocusTTY()]->writeRow + 1;
		ttyTable.listTTY[getFocusTTY()]->readCol = 0;
	}

	while( ttyTable.listTTY[getFocusTTY()]->readPointer != ttyTable.listTTY[getFocusTTY()]->writePointer ){
		
		if( ttyTable.listTTY[getFocusTTY()]->readPointer == SCREEN_SIZE ){
			ttyTable.listTTY[getFocusTTY()]->readPointer = 0;
		}
		character = (ttyTable.listTTY[getFocusTTY()]->stdOut->stdOutBuffer)[ttyTable.listTTY[getFocusTTY()]->readPointer];
		if( '\a' <= character && character <= '\r' )
		{
			refreshCharPrint[character - '\a'](SEND_TO_VIDEO, getFocusTTY());
		}else
		{
			printChar(character);
			incReadPointer( &(ttyTable.listTTY[getFocusTTY()]->readPointer), &(ttyTable.listTTY[getFocusTTY()]->readCol), &(ttyTable.listTTY[getFocusTTY()]->readRow));
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
	
	if( ttyTable.listTTY[tty]->writePointer == SCREEN_SIZE )
	{
		ttyTable.listTTY[tty]->writePointer = 0;
		ttyTable.listTTY[tty]->hasScrolled++;
	}
	parse = parseCharTTY(c, tty, inStdIn);
	if( parse )
	{
		incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );	
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
	
	if( nextTty == getFocusTTY() )
		return 1;
	clearScreen();
	ttyTable.focusTTY = ttyTable.listTTY[nextTty]->ttyId;
	setCursorPosition(0,0);
	refreshScreenTTY();
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
		//if (runningProcess->tty == getFocusTTY()){
		refreshKeyboardBufferTTY();
		//}
		refreshScreen();
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

