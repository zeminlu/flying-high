/*
 * ttys.c
 */

#include "ttys.h"

typedef void (*videoParser)( void );

typedef void (*printFunctions)( int, tty_t);

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
void getReadPointer(tty_t tty, int *readPointer, int *readCol, int *readRow){
	*readPointer = ttyTable.listTTY[tty]->readPointer;
	*readCol = ttyTable.listTTY[tty]->readCol;
	*readRow = ttyTable.listTTY[tty]->readRow;
}

void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow){
	*writePointer = ttyTable.listTTY[tty]->writePointer;
	*writeCol = ttyTable.listTTY[tty]->writeCol;
	*writeRow = ttyTable.listTTY[tty]->writeRow;
	
}

void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow){
	ttyTable.listTTY[tty]->readPointer = readPointer;
	ttyTable.listTTY[tty]->readCol = readCol;
	ttyTable.listTTY[tty]->readRow = readRow;
	
}

void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow){
	ttyTable.listTTY[tty]->writePointer = writePointer;
	ttyTable.listTTY[tty]->writeCol = writeCol;
	ttyTable.listTTY[tty]->writeRow = writeRow;	
}

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

static FILE * sysGetTTYFocusedProcessStream(tty_t tty, int fd){
	return processTable[sysGetTTYFocusedProcess(tty)].files[fd];
}

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

static void parseAlarmTTY( int where, tty_t tty )
{
}

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

	if ('\a' <= c && c <= '\r')
	{
		if (c == '\n' && inStdIn)
			putLine();
		else
			specialCharPrint[c - '\a'](WRITE_ON_TTY, tty);
		if (c == '\t' && inStdIn){
			++kbOffset[tty];
		}
		if ((inStdIn && c != '\n' && c != '\b') || (tty == focusTTY && !inStdIn)){
			videoParserFunctions[c - '\a']();
		}
		return 0;
	}
	else{
		((ttyTable.listTTY[tty])->stdOut->stdOutBuffer)[ttyTable.listTTY[tty]->writePointer] = c;
		if (inStdIn){
			ttyTable.listTTY[focusTTY]->stdIn->stdInBuffer[(ttyTable.listTTY[focusTTY]->stdIn->writeOffset)++]= c;
			++kbOffset[tty];
		}
		if (inStdIn || tty == focusTTY)
			printChar(c);
		incWritePointer( &(ttyTable.listTTY[tty]->writePointer), &(ttyTable.listTTY[tty]->writeCol), &(ttyTable.listTTY[tty]->writeRow) );		
		return 1;
	}
}

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
}

int getFocusedTTY( void ) 
{
	return ttyTable.focusTTY;
}
	
static void putTTY(Keycode c){
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
	tty_t tty;
	
	tty = getFocusedTTY();
	if(fwrite(sysGetTTYFocusedProcessStream(tty, STDIN), (char *)&c, 1) == 1){
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
			}
			else if( deChar == 0x8A ){
				kill(ttyTable.listTTY[ttyTable.focusTTY]->focusProcess);
				/* HACER LA SEÑAL DE MATAR AL PROCESO */
			}
			else{
				if(runningProcess->ttyMode  == TTY_CANONICAL){
					putCharTTY((int)deChar, getFocusedTTY(), TRUE);
				}
				else{
					putTTY((int)deChar);
				}
			}
		}
	}
}

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

void refreshStdout(void){
	char aux;

	while(read(STDOUT, &aux, 1 ) == 1){
		putCharTTY(aux, runningProcess->tty, FALSE);
	}

}

void refreshTTY(void){

	if(runningProcess != NULL && runningProcess != initProcess && runningProcess->pid > 0 && runningProcess->tty != -1){
		refreshKeyboardBufferTTY();
		refreshStdout();
		refreshScreen();
	}
	
	return;
}

/*
 *Public Functions
 */

void sysSetTTYFocusedProcess(pid_t pid, tty_t tty){
	ttyTable.listTTY[tty]->focusProcess = pid; 
}

pid_t sysGetTTYFocusedProcess(tty_t tty){
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

void clearTTYScreen( tty_t tty )
{
	int clear = 0;
	
	while( clear != SCREEN_SIZE )
	{
		(ttyTable.listTTY[tty]->stdOut->stdOutBuffer)[clear] = ' ';
		clear++;
	}
	clearScreen();
}

void setTTYCursorPosition( int x, int y, tty_t tty)
{
	
	if( x >= SCREEN_HEIGTH || y >= SCREEN_WIDTH )
		return;
	
	ttyTable.listTTY[tty]->writePointer = SCREEN_WIDTH * x + y;
	ttyTable.listTTY[tty]->writeRow = x;
	ttyTable.listTTY[tty]->writeCol = y; 
}

void putCharATTTYPosition( int c, int row, int col, tty_t tty)
{
	int auxPointer, auxCol, auxRow;
	if( row >= SCREEN_HEIGTH || col >= SCREEN_WIDTH )
		return;
	
	auxPointer = ttyTable.listTTY[tty]->writePointer;
	auxRow = ttyTable.listTTY[tty]->writeRow;
	auxCol = ttyTable.listTTY[tty]->writeCol;
	setTTYCursorPosition(row, col, tty);
	ttyTable.listTTY[tty]->writePointer = auxPointer;
	ttyTable.listTTY[tty]->writeRow = auxRow;
	ttyTable.listTTY[tty]->writeCol = auxCol;
	
	putCharTTY(c, tty, FALSE);
	if( tty == getFocusedTTY() ){
		putCharAtFixedPos(c, getVideoColor(), row, col );
		refreshScreen();
	}
}

