
/*
 * 	File :mouseInit.h
 * 	Author: Lucas Pizzagalli
 * 	Version: 02/06/2009 - 12:17
 * 	Description:
 * 			Header of the mouse initialization, for be ready to listen 
 * 			from ports the mouse events. 
 *
 *
 */

#ifndef MOUSEINIT_H_
#define MOUSEINIT_H_
/*
 * mouseinit
 * 	Routine needed to start mouse usage.
 * 	-It sends start configuration instruction(A8h) to 64h
 *	-Sends 20h comand to port 64h in order to get status byte
 *	- Read the status byte in port 60h and habilitates mouse bit(bit 1)
 *	- sends comand 60h to port 64h to be hable to send modified status byte
 *	- send that status byte
 *	- and write to mouse configurations (comands F6h and F4h)
 * 	
 */

void mouseinit();

#endif
