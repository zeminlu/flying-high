/* File: KBDriver.c
 *
 */

/*
 * Includes
 */
#include "videoDriver.h"
#include "KBDriver.h"
#include "stdio.h"
#include "tty.h"
#include "sysprocess.h"

unsigned char KBBuffer[SCREEN_SIZE];
int KBBOffSetRead = 0;
int KBBOffSetWrite= 0;
int keysToTake = 0;
int isAbleToRead = 0;

/* Function: int16
 * Description:
 * 		If the screen-saver is on, it shuts down.
 * 		Else, bufferize and print on screen the 
 * 		cunsigned haracter recived.
 * Parameters:
 * 		c: a unsigned character
 * Programmer:
 * 		Santiago Bermudez
 * Version:	
 * 		24.05.09.01.54
 */


void int16( int s ) {
	unsigned char c = (unsigned char) s;
	tty_t backupTTY;
	if( c & CHANGETTYMASK ){ /* if F */
		ttyPutchar(c);
		return;
	}
	if(c){
		backupTTY = sysGetTty(sysGetpid());
		sysSetTty(sysGetpid(), getFocusTTY());
		if ( c != 0x08 ) { /* If is not backspace */
			if ( c == 0x0D ) {/* Carriage return */
				isAbleToRead = TRUE;
				++keysToTake;
				if (keysToTake >= SCREEN_SIZE - 1)
					keysToTake = SCREEN_SIZE -1;
				ttyPutchar('\n');	
				KBBuffer[KBBOffSetWrite++] = '\n';

				KBBOffSetWrite %= SCREEN_SIZE;
//				puts("Signal to: ");
				//puti(getFocusTTY());
				//puts("\n");
				signalTty(getFocusTTY()/*sysGetTty(sysGetpid())*/);
			}
			else {
				isAbleToRead=0;
				++keysToTake;	
				if (keysToTake >= SCREEN_SIZE - 1)
					keysToTake = SCREEN_SIZE -1;
				ttyPutchar(c);
				KBBuffer[KBBOffSetWrite++] = c;

				KBBOffSetWrite %= SCREEN_SIZE;

			}
		}
		else if (keysToTake > 0){
			if ( ttyBackSpaceOnScreen()){
				--keysToTake;
				--KBBOffSetWrite;
				if ( KBBOffSetWrite < 0 )
					KBBOffSetWrite = SCREEN_SIZE - 1;
			}
		}
	sysSetTty(sysGetpid(), backupTTY);
	}

}
unsigned char readKBChar( void ) {
	unsigned char c = EOF;
	if ( keysToTake > 0 && (isAbleToRead == 1) ) {
		keysToTake--;
		c = KBBuffer[KBBOffSetRead];
		KBBOffSetRead++;

		KBBOffSetRead %= SCREEN_SIZE;

	}
	if ( (!keysToTake) || (c == '\n'))
		isAbleToRead = 0; 
	return c;
}

int readKBString( unsigned char * string, size_t size ){
	int i;
	for(i = 0; i < size && isAbleToRead; ++i)
		string[i] = readKBChar(); 
	return i;
}

void pasteInKB( unsigned char * pasteBuf, int size){
	int i;
	for( i = 0; i < size; ++i ){
		isAbleToRead=0;
		++keysToTake;	
		KBBuffer[KBBOffSetWrite++] = pasteBuf[i];
		KBBOffSetWrite %= SCREEN_SIZE;
	}
	KBBuffer[KBBOffSetWrite]= '\0';
//	puts(&KBBuffer[KBBOffSetWrite - i]);
	if (keysToTake >= SCREEN_SIZE - 1)
		keysToTake = SCREEN_SIZE -1;
}

void clearKBBuff(){
	KBBOffSetRead = 0;
	KBBOffSetWrite= 0;
	keysToTake = 0;
	isAbleToRead = 0;
}

int isTTYAbleToRead(){
	return isAbleToRead;
}
