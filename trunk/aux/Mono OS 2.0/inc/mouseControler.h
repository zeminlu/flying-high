/*
 * FileName: 
 * 			mouseControler.c
 * 	Version:
 * 			02.06.2009
 * 	Author:
 * 			Lucas Pizzagalli
 * 	Description:
 * 			contains the mouse controler and auxilieary functions.
 * 			This controlers handl the botton press of the mouse
 * 			and act in line.
 */
#ifndef MOUSECONTROLLER_H_
#define MOUSECONTROLLER_H_
/* 
 *  * Include project section 
 *  
 */
/*
#include "../src/videoDriverRef.h"
*/
/*
 * DEFINES
 */

#define TRUE 1
#define FALSE !TRUE
#define SELECTION_COLOR BLAK_F | LIGHT_GREY 


/*
 *mouse_controler:
 *		Description: Handles the mouse clics. 
 *					* If the left botton is presed, starts or 
 *					  continues copying from screen useing
 *					  the clipboard.
 *					* If the right botton is presed, the buffer 
 *					  copied in clipboard is copied to screen.
 *		Recibes:
 *					* flags for mouse.
 *
 */
void mouseControler(int , int, int);

#endif
