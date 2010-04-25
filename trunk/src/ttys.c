/*
 * ttys.c
 */

#include "ttys.h"

typedef struct TTY{
	tty_t	ttyId;
	Keycode	KBBuffer[KEYBOARD_BUFFER_SIZE];
	unsigned char TerminalBuffer[VIDEO_MEMORY_SIZE];
	pid_t	focusProcess;
}TTY;

typedef struct sysTTY{
	int		qtyTTY;
	TTY		ttys[MAX_TTY];
	tty_t	focusTTY;
}sysTTY;

sysTTY ttyTable;

void initializeTTY( void )
{
	int i;
	
	for( i = 0 ; i < MAX_TTY ; ++i )
	{
		if( i > 0 )
			memcpy(ttyTable.ttys[i].TerminalBuffer, "root@ArkOS$>", 13 ); 
		ttyTable.ttys[i].ttyId = i;
	}
	ttyTable.qtyTTY = MAX_TTY;
	ttyTable.focusTTY = 0;
	ttyTable.ttys[0].focusProcess = 0;		/* Cuando arranca la TTY tiene al Idle corriendo */
	keyboardBuffer = ttyTable.ttys[0].KBBuffer;
	videoBuffer = ttyTable.ttys[0].TerminalBuffer;
}

int getCurrentTty( void ) 
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
	keyboardBuffer = ttyTable.ttys[nextTty].KBBuffer;
	KBCleaner();
	clearScreen();
	memcpy(videoBuffer, ttyTable.ttys[nextTty].TerminalBuffer, VIDEO_MEMORY_SIZE);
	refreshScreen();
	return 0;
}

static void KBCleaner ( void )
{
	int i;
	
	for( i = 0 ; i < KEYBOARD_BUFFER_SIZE ; ++i )
	{
		ttyTable.ttys[getCurrentTty()].KBBuffer[i] = '\0';
	}
}