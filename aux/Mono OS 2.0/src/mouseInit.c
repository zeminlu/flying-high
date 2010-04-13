/*
 * 	File :mouseInit.c
 * 	Author: Lucas Pizzagalli
 * 	Version: 02/06/2009 - 12:17
 * 	Description:
 * 			Is the mouse initialization, for be ready to listen 
 * 			from ports the mouse movements. 
 * information taken from:
 * 		http://www.houbysoft.com/download/ps2mouse.html
 *
 */

/*
 *		Include Section
 */

#include "videoDriver.h"
#include "portsHandler.h"

/*
 *		Define Section
 *
 */

#define MOUSE_HAS_READ 0
#define MOUSE_READ 1
#define MOUSE_WRITE 2

/*
 *		FUNCTIONS!
 *
 */

/*
 *		internal functions
 */

/*
 * mouse_wait:
 * 		This function make the procesor to wait for mouse to be ready
 * 		to Write, Read or Writen data has already been procesed by the mouse
 * 
 * recibes:
 * 		MOUSE_WRITE --> wait mouse to be ready to written
 * 		MOUSE_READ --> wait mouse to be ready to read from
 * 		MOUSE_HAS_READ --> wait until mouse has procesed written data
 */
static void
mouseWait(unsigned char option)
{
	unsigned int time = 10000;
	while( time )
	{	
		if(option == MOUSE_WRITE)
		{
			if(((inb(0x64)& 2) == 0))
				return;
		}
		if(option == MOUSE_READ)
		{
			if(((inb(0x64)& 1) == 1))
				return;
		}
		if(option == MOUSE_HAS_READ)
		{
			if( (inb(0x64) == 0xFA))
				return;
		}
		--time;
	}
}

/*
 *mouseWrite:
 *		This function facilitate the writes to de mouse, to port
 *		60h. It sends write package D4h and write an unsigned
 *		char to mouse.
 *	
 * recibes:
 * 		unsigned char wanted to be written
 */

static void
mouseWrite(unsigned char a)
{
	mouseWait(MOUSE_WRITE);
	outb(0x64, 0xD4);
	mouseWait(MOUSE_WRITE);
	outb(0x60,a);
	mouseWait(MOUSE_HAS_READ);
	return;
}

/*
 *		Public Function
 *
 */

/*
 * mouseinit
 * 		Routine needed to start mouse usage.
 * 		-It sends start configuration instruction(A8h) to 64h
 *		-Sends 20h comand to port 64h in order to get status byte
 *		- Read the status byte in port 60h and habilitates mouse bit(bit 1)
 *		- sends comand 60h to port 64h to be hable to send modified status byte
 *		- send that status byte
 *		- and write to mouse configurations (comands F6h and F4h)
 * 	
 */

void
mouseinit()
{
	unsigned char status;
	mouseWait(MOUSE_WRITE);
	outb(0x64,0xA8);
	mouseWait(MOUSE_HAS_READ);
	mouseWait(MOUSE_WRITE);
	outb(0x64, 0x20);
	mouseWait(MOUSE_HAS_READ);
	mouseWait(MOUSE_READ);
	status = (inb(0x60) | 2);
	mouseWait(MOUSE_WRITE);
	outb(0x64,0x60);
	mouseWait(MOUSE_HAS_READ);
	mouseWait(MOUSE_WRITE);
	outb(0x60, status);
	mouseWrite(0xF6);
	mouseWrite(0xF4);
	turnOnMouse(0xFF);
	return;
}	
