/*
 * ttys.c
 */

#include "ttys.h"

typedef struct TTY{
	tty_t	ttyId;
	Keycode *	stdin;
	unsigned char TerminalBuffer[VIDEO_MEMORY_SIZE];
	pid_t	focusProcess;
}TTY;

typedef struct sysTTY{
	int		qtyTTY;
	TTY		ttys[MAX_TTY];
	tty_t	focusTTY;
}sysTTY;

sysTTY ttyTable;

void printToScreen(char * str, int amm);

void initializeTTY( void )
{
	int i;
	
	for( i = 0 ; i < MAX_TTY ; ++i )
	{
		memcpy(ttyTable.ttys[i].TerminalBuffer, "root@ArkOS$>", 13 ); 
		ttyTable.ttys[i].ttyId = i;
	}
	ttyTable.qtyTTY = MAX_TTY;
	ttyTable.focusTTY = 0;
	ttyTable.ttys[0].focusProcess = 0;		/* Cuando arranca la TTY tiene al Idle corriendo */
	ttyTable.ttys[0].stdin = keyboardBuffer;
	
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



	