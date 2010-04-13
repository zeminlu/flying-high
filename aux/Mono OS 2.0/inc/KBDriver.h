/* File: KBDriver.h
 *
 */

#ifndef KBDRIVER_H_
#define KBDRIVER_H_

#include "stdio.h"
#ifndef EOF
#define EOF 255 
#endif

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

void int16( int s );

/*
 * Function: readKBChar
 * Description: Reads a char
 */
unsigned char readKBChar( void );

/*
 * Function: pasteInKb
 * Description: Pästes in the keyboard buffer the given buffer of the
 * given size
 */
void pasteInKB( unsigned char * pasteBuf, int size);

/*
 * Function: readKBString
 * Description: Reads a string to the given buffer of the given size.
 * returns the amount of characters read
 */
int readKBString( unsigned char * string, size_t size );

void clearKBBuff();

int isTTYAbleToRead();
#endif
