/*
 * File: portsHandlers.h
 * Description:
 * 		Basic functions to manange I/O ports in low level functions
 *
 */

#ifndef PORTSHANDLER_H_
#define PORTSHANDLER_H_
/*
* Function: outb
* Description:
*		Writes to port
*		 
* Recives:	
*		port
*		char to be written
*		
* Programmer:
*		Lucas Pizzagalli
; Version:	
*		24.05.09.18.41
*/

void outb(int port , char algo);

/*
* Function: outb
* Description:
*		Writes to port
*		 
* Recives:	
*		port
* returns: 
*		a byte read from recived port
*
* Programmer:
*		Lucas Pizzagalli
; Version:	
*		24.05.09.18.41
*/


unsigned char inb(int port);

#endif
