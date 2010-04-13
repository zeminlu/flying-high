
/* 
 *   File: mouse.c
 *   Author: Lucas Pizzagalli
 *   Version: 02/06/2009 - 15:17
 *   Description: 
 *   		Is the mouse Driver, it get all the data from mouse 
 *   		and sends the it to de diferent controlers
 */

#ifndef MOUSE_H_
#define MOUSE_H_

/*
 * Inlcude Section
 */


/*
 * mouse:handler
 * 		Description:
 * 			get from ports, the 3 bytes with all the data of  
 * 			mouse movements. With this information, it comunicates
 * 			actions to de mouse controler and to de video controler.
 *			It is called from int 74h interuption
 */

void mouseHandler(void );

#endif
