/*
*
*	File: clipboardManager.h
*	Author: Santiago Bermudez, Santiago Perez De Rosso, Lucas Pizzagalli
*	Description: Administrates the clipboard
*	
*/

#ifndef CLIPBOARDMANAGER_H_
#define CLIPBOARDMANAGER_H_
/*
 *	Function: void copy(int xInf, int xSup, int yInf, int ySup)
 *	Description: Copies the area delimited by the boundaries given
 *	Parameters:
 *		ySup ------------------------
 *		     |                      |
 *                   |                      |
 * 		yInf ------------------------
 *		    xInf                  xSup
 *	
 */
void copy(int xInf, int xSup, int yInf, int ySup);

/*
 *	Function: void paste()
 *	Description: Pastes the clipboard information
 */
void paste();

#endif
