/**
 *	\file ttys.c
 *
 *		\brief The tty module implementation.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "ttys.h"

typedef void (*videoParser)( void );

typedef void (*printFunctions)( int, tty_t);

extern unsigned char videoBuffer[VIDEO_MEMORY_SIZE];

extern process_t processTable[MAX_PROCESS];

extern process_t *runningProcess;

extern process_t *initProcess;

extern Keycode *keyboardBuffer;

static sysTTY ttyTable;

static tty_s TTYsList[MAX_TTY];

static stdInTTY TTYstdIns[MAX_TTY];

static stdOutTTY TTYstdOuts[MAX_TTY];

static Keycode stdInBuffers[MAX_TTY][MAX_LINE];

static Keycode stdOutBuffers[MAX_TTY][SCREEN_SIZE];

static unsigned int kbOffset[MAX_TTY];

static videoParser videoParserFunctions[] = {
	printAlarm,
	printBackspace,
	printTab,
	printNewLine,
	printVTab,
	printLineFeed,
	printReturn
};
/* Initialize the structures of each TTYs */
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
		ttyTable.listTTY[i]->writePointer = 0;
		ttyTable.listTTY[i]->writeCol = CURSOR_START_COL;
		ttyTable.listTTY[i]->writeRow = CURSOR_START_ROW;
		ttyTable.listTTY[i]->readPointer = 0;
		ttyTable.listTTY[i]->readCol = CURSOR_START_COL;
		ttyTable.listTTY[i]->readRow = CURSOR_START_ROW;
		kbOffset[i] = 0;
	}
}

/*
*	Static functions for putcharTTY
*/

/* Obtain the read pointer of the specific TTY */
void getReadPointer(tty_t tty, int *readPointer, int *readCol, int *readRow){
	*readPointer = ttyTable.listTTY[tty]->readPointer;
	*readCol = ttyTable.listTTY[tty]->readCol;
	*readRow = ttyTable.listTTY[tty]->readRow;
}

/* Obtain the write Pointer of the specific TTY */
void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow){
	*writePointer = ttyTable.listTTY[tty]->writePointer;
	*writeCol = ttyTable.listTTY[tty]->writeCol;
	*writeRow = ttyTable.listTTY[tty]->writeRow;
	
}

/* Backup the reads variables of the tty */
void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow){
	ttyTable.listTTY[tty]->readPointer = readPointer;
	ttyTable.listTTY[tty]->readCol = readCol;
	ttyTable.listTTY[tty]->readRow = readRow;
	
}

/* Backup the writers variables of the tty */
void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow){
	ttyTable.listTTY[tty]->writePointer = writePointer;
	ttyTable.listTTY[tty]->writeCol = writeCol;
	ttyTable.listTTY[tty]->writeRow = writeRow;	
}

/* increment the read pointers */
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

/* decrement the read pointers */
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

/* increment the write pointers */
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

/* decrement the read pointers */
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

/* return the FILE * of the specific tty */
static FILE * sysGetTTYFocusedProcessStream(tty_t tty, int fd){
	return processTable[sysGetTTYFocusedProcess(tty)].files[fd];
}

/* send to the buffer shell the enter line */
static void putLine(void)
{
	int tty;
	char nl = '\n';
	
	tty = getFocusedTTY();
	fwrite(sysGetTTYFocusedProcessStream(tty, STDIN), (char *)(ttyTable.listTTY[tty]->stdIn->stdInBuffer), ttyTable.listTTY[tty]->stdIn->writeOffset);
	fwrite(sysGetTTYFocusedProcessStream(tty, STDIN), &nl, 1);
	signalTty(tty);
	ttyTable.listTTY[tty]->stdIn->writeOffset = 0;
	kbOffset[tty] = 0;
}

/* Parse the alarm control character */
static void parseAlarmTTY( int where, tty_t tty )
{
}

/* Parse the backspace control character */
static void parseBackSpaceTTY( int where, tty_t tty )
{
	static int cond = FALSE;
	int i;
	
	if (!where)
	{	
		if (kbOffset[tty] > 0){
			cond = FALSE;
			--kbOffset[tty];
			decWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow));
			(ttyTable.listTTY[tty]->stdIn->writeOffset)--;
			if ((ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] == TAB){
				for (i = 0 ; i < VIDEO_TAB_STOP - 1 ; ++i){
					if ((ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] != TAB)
						break;
					--kbOffset[tty];
					printBackspace();
					(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = ' ';
					decWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow));
					--(ttyTable.listTTY[tty]->stdIn->writeOffset);
				}
				cond = TRUE;
			}
			if (!cond || (cond && i == VIDEO_TAB_STOP - 1)){
				printBackspace();
				(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = ' ';
			}
		}
	}
	else{
		if (cond){
			for (i = 0 ; i < VIDEO_TAB_STOP ; ++i)
				printBackspace();
			cond = FALSE;
		}
		else
			printBackspace ();
	}
	
	return;
}

/* Parse the tab control character */
static void parseTabTTY( int where, tty_t tty )
{	
	int i, prevRow;
	
	if( !where ){
		for ( i = 0, prevRow = ttyTable.listTTY[tty]->writeRow; i < VIDEO_TAB_STOP ; ++i )
		{
			if ( prevRow != ttyTable.listTTY[tty]->writeRow )
				break;
			(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = TAB;
			incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );		
		}
	}
	else{
		printChar(' ');
		incReadPointer( &(ttyTable.listTTY[tty]->readPointer), &(ttyTable.listTTY[tty]->readCol), &(ttyTable.listTTY[tty]->readRow));
	}
}

/* Parse the newline control character */
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

/* Parse the vertical tab control character */
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

/* Parse the line feed control character */
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

/* Parse the return control character */
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

/* Parse the character and determine what it has to do */
static int parseCharTTY( int c, tty_t tty, int inStdIn)
{
	tty_t focusTTY;
	static printFunctions specialCharPrint[] = {
		parseAlarmTTY,
		parseBackSpaceTTY,
		parseTabTTY,
		parseNewLineTTY,
		parseVTabTTY,
		parseLineFeedTTY,
		parseReturnTTY
	};
	
	focusTTY = getFocusedTTY();

	if ('\a' <= c && c <= '\r' && c != '\n')
	{
		if (!inStdIn || (inStdIn && c != '\t' && (c != '\t' || kbOffset[tty] < MAX_LINE - VIDEO_TAB_STOP - 1)) ){
		
			specialCharPrint[c - '\a'](WRITE_ON_TTY, tty);
		
			if (c == '\t' && inStdIn){
				kbOffset[tty] += VIDEO_TAB_STOP;
			}
			if ((inStdIn && c != '\b') || (tty == focusTTY && !inStdIn)){
				videoParserFunctions[c - '\a']();
			}
		}
		
		return 0;
	}
	else if (c == '\n'){
		if (inStdIn)
			putLine();
		else
			specialCharPrint[c - '\a'](WRITE_ON_TTY, tty);
		if (tty == focusTTY && !inStdIn){
			videoParserFunctions[c - '\a']();
		}
		return 0;
	}
	else{
		if (!inStdIn || (inStdIn && kbOffset[tty] < MAX_LINE - 1)){
			((ttyTable.listTTY[tty])->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = c;
			if (inStdIn){
				ttyTable.listTTY[focusTTY]->stdIn->stdInBuffer[(ttyTable.listTTY[focusTTY]->stdIn->writeOffset)++]= c;
				++kbOffset[tty];
			}
			if (inStdIn || tty == focusTTY)
				printChar(c);
			incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );		
		}
		return 1;
	}
}

/* Refresh all buffers of the tty */
static void refreshScreenTTY( void )
{
	int character;
	tty_t focusTTY;
	static printFunctions refreshCharPrint[] = {
		parseAlarmTTY,
		parseBackSpaceTTY,
		parseTabTTY,
		parseNewLineTTY,
		parseVTabTTY,
		parseLineFeedTTY,
		parseReturnTTY
	};
	
	focusTTY = getFocusedTTY();
	
	ttyTable.listTTY[focusTTY]->readPointer = ttyTable.listTTY[focusTTY]->readCol = ttyTable.listTTY[focusTTY]->readRow = 0;
	if( ttyTable.listTTY[focusTTY]->hasScrolled > 0 ){
		
		ttyTable.listTTY[focusTTY]->readPointer = ttyTable.listTTY[focusTTY]->writePointer;
		ttyTable.listTTY[focusTTY]->readCol = ttyTable.listTTY[focusTTY]->writeCol;
		do{
			incReadPointer( &(ttyTable.listTTY[focusTTY]->readPointer), &(ttyTable.listTTY[focusTTY]->readCol), &(ttyTable.listTTY[focusTTY]->readRow));
		}while( ttyTable.listTTY[focusTTY]->readCol != 0 );
		
		ttyTable.listTTY[focusTTY]->readRow = ttyTable.listTTY[focusTTY]->writeRow + 1;
		ttyTable.listTTY[focusTTY]->readCol = 0;
	}

	while( ttyTable.listTTY[focusTTY]->readPointer != ttyTable.listTTY[focusTTY]->writePointer ){
		
		if( ttyTable.listTTY[focusTTY]->readPointer == SCREEN_SIZE ){
			ttyTable.listTTY[focusTTY]->readPointer = 0;
			ttyTable.listTTY[focusTTY]->readCol = 0;
			ttyTable.listTTY[focusTTY]->readRow = 0;
		}
		character = (ttyTable.listTTY[focusTTY]->stdOut->stdOutBuffer)[ttyTable.listTTY[focusTTY]->readPointer];
		if( '\a' <= character && character <= '\r' )
		{
			refreshCharPrint[character - '\a'](SEND_TO_VIDEO, focusTTY);
		}else
		{
			printChar(character);
			incReadPointer( &(ttyTable.listTTY[focusTTY]->readPointer), &(ttyTable.listTTY[focusTTY]->readCol), &(ttyTable.listTTY[focusTTY]->readRow));
		}
	}
}

/* Print a stirng in the specific tty */
void putsTTY( unsigned char *name, int count, tty_t tty )
{
	while( count-- >= 0 )
		putCharTTY(*name++, tty, TRUE);
}

/* Put a char into the tty and verifies if it has to put in Stdin too */
void putCharTTY( char c, tty_t tty, int inStdIn )
{	
	int parse;
	
	if( ttyTable.listTTY[tty]->writePointer == SCREEN_SIZE )
	{
		ttyTable.listTTY[tty]->writePointer = 0;
		ttyTable.listTTY[tty]->hasScrolled++;
	}
	parse = parseCharTTY(c, tty, inStdIn);
}

/* return the id of the tty that is in focus */
int getFocusedTTY( void ) 
{
	return ttyTable.focusTTY;
}

/* put a char into the tty that is in focus */
static void putTTY(Keycode c){
	tty_t tty;
	
	tty = getFocusedTTY();
	if(fwrite(sysGetTTYFocusedProcessStream(tty, STDIN), (char *)&c, 1) == 1){
		signalTty(tty);
	}	
}

/* dequeue characters from the keyboard buffer, and decides if it has to putchar or change tty */
void refreshKeyboardBufferTTY( void ){
	Keycode deChar = 0;
	int color;
	
	color = getVideoColor();

	while( !kbBufferIsEmpty() ){
		if( (deChar = charDeque()) != '\0' )
		{
			if( 0x81 <= deChar && deChar <= 0x88  ){
				changeFocusTTY( (deChar - 0x81) );
				break;
			}
			else if( deChar == 0x8A ){
				kill(ttyTable.listTTY[ttyTable.focusTTY]->focusProcess);
			}
			else{
				if(getProcessTable()[ttyTable.listTTY[ttyTable.focusTTY]->focusProcess].ttyMode  == TTY_CANONICAL){
					putCharTTY((int)deChar, getFocusedTTY(), TRUE);
				}
				else{
					putTTY((int)deChar);
				}
			}
		}
	}
}

/* Change the focus tty */
int changeFocusTTY( tty_t nextTty ){	
	
	if( nextTty == getFocusedTTY() )
		return 1;
	clearScreen();
	ttyTable.focusTTY = ttyTable.listTTY[nextTty]->ttyId;
	setCursorPosition(0,0);
	refreshScreenTTY();
	refreshScreen();
	return 0;
}

/* Put chars into the STDOUT of the running process */
void refreshStdout(void){
	char aux;

	while(read(STDOUT, &aux, 1 ) == 1){
		putCharTTY(aux, runningProcess->tty, FALSE);
	}

}

/* Refresh focus tty */
void refreshTTY(void){

	refreshScreen();
	if(runningProcess != NULL && runningProcess != initProcess && runningProcess->pid > 0 && runningProcess->tty != -1){
		refreshStdout();
	}
	
	return;
}

/*
 *Public Functions
 */

/* Set the pid to the process that is in focus, for this tty */
void sysSetTTYFocusedProcess(pid_t pid, tty_t tty){
	ttyTable.listTTY[tty]->focusProcess = pid; 
}
 /* get the pid of the process that is in focus of this tty */
pid_t sysGetTTYFocusedProcess(tty_t tty){
	return ttyTable.listTTY[tty]->focusProcess;
}

/* Clean the stdout buffer of the running process tty */
void clearTTYScreen()
{
	int clear = 0;
	tty_t tty;
	
	if ((tty = runningProcess->tty) < 0 || tty > MAX_TTY)
		return;
	
	while( clear != SCREEN_SIZE )
	{
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[clear] = ' ';
		clear++;
	}
	setTTYCursorPosition(0, 0);
	if (tty == getFocusedTTY()){
		clearScreen();
		setCursorPosition(0, 0);
	}
}

/* Put the pointer in x,y position */
void setTTYCursorPosition( int x, int y)
{
	tty_t tty;
	
	tty = runningProcess->tty;
	
	if( x >= SCREEN_HEIGTH || y >= SCREEN_WIDTH || tty < 0 || tty > MAX_TTY)
		return;
		
	ttyTable.listTTY[tty]->writePointer = SCREEN_WIDTH * x + y;
	ttyTable.listTTY[tty]->writeRow = x;
	ttyTable.listTTY[tty]->writeCol = y;
	if (tty == getFocusedTTY()){
		setCursorPosition(x, y);
	}
}

/* put chat at tty position buffer */
void putTTYCharAtPosition( int c, int row, int col)
{
	int auxPointer, auxCol, auxRow;
	tty_t tty;
	
	tty = runningProcess->tty;
	
	if( row >= SCREEN_HEIGTH || col >= SCREEN_WIDTH || tty < 0 || tty > MAX_TTY)
		return;
	
	auxPointer = ttyTable.listTTY[tty]->writePointer;
	auxRow = ttyTable.listTTY[tty]->writeRow;
	auxCol = ttyTable.listTTY[tty]->writeCol;
	setTTYCursorPosition(row, col);
	putCharTTY(c, tty, FALSE);
	if( tty == getFocusedTTY() ){
		putCharAtCurrentPos(c, getVideoColor());
		refreshScreen();
	}
	setTTYCursorPosition(auxRow, auxCol);
	
	
	return;
}

