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

/*
 * 	Include section
 */
#include "mouseControler.h"
#include "clipboardManager.h"
#include "videoDriver.h"
/*
 *	GLOBAL VARIABLES SECTION
 *
 */

int startX;
int startY;
int endX;
int endY;
int isCopying;
int hasCopyed;

/*
 * swap:
 * 		Description: 
 * 				swap 2 integer pointers.
 * 		recives:
 * 				a : pointer to an integer
 * 				b: pointer to the oder integer
 *
 */

static void
swap( int *a, int *b)
{
	int *aux = 0;
	*aux = *a;
	*a = *b;
	*b = *aux;
}
/*
 *	possOrder:
 *		Description: 
 *				Re-order the parameters in order to get:
 *				(inferiorX, superiorX, inferiorY, supperiorY(
 *		recives:
 *				x1: pointer to one X.
 *				x2: pointer to the oder X.
 *				y1: pointer to one Y.
 *				y2: pointer to the oder Y.
 *
 */
static void
possOrder( int *x1, int *x2, int *y1, int *y2)
{
	if(*x1 > *x2)
		swap(x1,x2);
	if( *y1 > *y2)
		swap(y1,y2);
}


/*
 *mouseControler:
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


void
mouseControler(int flags, int x, int y)
{
	int xI, xS, yI, yS;
	if((flags & 3) == 3 )
		return;
	if( flags & 1)
	{
			if( isCopying )
			{
				xI = startX;
				yI = startY;
				endX = xS = getMouseX();
				endY = yS = getMouseY();  	
				possOrder(&xI, &xS, &yI, &yS);
				screenColor(NO_MOUSE_FORMAT);
				screenColorArea(xI, xS, yI, yS, MAGENTA_B | BLACK_F);
			}
			else
			{
				endX = startX = getMouseX();
				endY = startY = getMouseY();
				screenColor(NO_MOUSE_FORMAT);
				screenColorArea(startX, startX, startY, startY, MAGENTA_B | BLACK_F);
				isCopying = TRUE;
				screenColor(NO_MOUSE_FORMAT);
			}
	}
	else if( flags & 2 && x == 0 && y == 0)
	{
			if( hasCopyed )
			{
				paste();
				screenColor(NO_MOUSE_FORMAT);
			}
	}
	else if ( isCopying == TRUE )
	{
		possOrder(&startX, &endX, &startY, &endY);
		screenColorArea(startX, endX, startY, endY, MAGENTA_B|BLACK_F);
		copy(startX, endX, startY, endY);
		isCopying = FALSE;
		hasCopyed = TRUE;
	}
	return;
}





