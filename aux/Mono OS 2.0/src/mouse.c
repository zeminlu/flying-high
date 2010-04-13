/* 
 *   File: mouse.c
 *   Author: Lucas Pizzagalli
 *   Version: 02/06/2009 - 15:17
 *   Description: 
 *   		Is the mouse Driver, it get all the data from mouse 
 *   		and sends the it to de diferent controlers
 */



/*
 * Inlcude Section
 */


#include "mouseControler.h"
#include "mouse.h"
#include "portsHandler.h"
#include "videoDriver.h"


/*
 * EXTERN DECLARATIONS
*/
extern void mouse_int_served();


	
/*
 * mouseIsReady:
 * 		
 * Description:
 * 		wait for mouse to be ready to read
 *
 */
static void
mouseIsReady(void )
{
	int status;
	int time = 100000;
	while( time)
 	{
		if((((status = inb(0x64)) & 0x21)== 0x21) )
			return;
		time--;
	}
	return;	

}
/*
 * mouse:handler
 * 		Description:
 * 			get from ports, the 3 bytes with all the data of  
 * 			mouse movements. With this information, it comunicates
 * 			actions to de mouse controler and to de video controler.
 *			It is called from int 74h interuption
 */

void
mouseHandler(void )
{
	int flags,X_delta,Y_delta;
	flags = inb(0x60);				//Read flags from port 60h
	mouse_int_served();				//mouse interuption is served(20h to PIC1/2)
	mouseIsReady();					//
	X_delta = inb(0x60);
	mouse_int_served();
	mouseIsReady();
	Y_delta = inb(0x60);
	mouse_int_served();
	if((flags & 0xC0) != 0)
	{
		return;
	}
	
	if(X_delta > 0)
	{
		if((flags & 16) == 0)
		{
			X_delta = 1;
		}
		else
		{
		
			X_delta = -1;
		}
	}
	if(Y_delta > 0)
	{
		if((flags & 32) == 0)
			Y_delta = -1;
		else
			Y_delta = 1;
	}
	mouseSet(X_delta, Y_delta);
	mouseControler(flags, X_delta, Y_delta);
	return ;
}
