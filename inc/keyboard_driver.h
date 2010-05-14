#ifndef _KEYBOARD_DRIVER_H_
#define _KEYBOARD_DRIVER_H_

/*
 *  keyboard_driver.h
 */ 

#include "queue.h"
#include "io.h"
#include "config.h"
#include "ttys.h"

/*
 *	Functions Section
 */

/*
 * \fn void keyboard_driver ( void )
 *
 * \brief This function is the manager of interpretate what key 
 * was pressed or released. It controls if the own keyboard buffer
 * is full. The keys that are implemented are:
 * 		- Letters ( upper and lower case )
 * 		- Numbers ( If you press SHIFT it will interpretate the symbol
 * 					associated with this key )
 * 		- SHIFT buttom
 * 		- CAPS-LOCK
 *
 */

void keyboard_driver( void );

/*
 *
 *  \fn void SetKeyState(Keycode scancode )
 *
 *  \brief 	This function detect if a control key was pressed and set his
 *  		internal flag.
 */

void SetKeyState( Keycode  scanCode );

/*
 *
 * \fn int shiftPressed(void)
 *
 * \brief Return TRUE if the shift key was pressed
 */

int shiftIsPressed( void );

/*
 *
 * \fn int capsIsPressed(void)
 *
 * \brief Return TRUE if caps-lock key was pressed
 */


int capsIsPressed ( void );

/*
 *
 * \fn int ctrlPressed(void)
 *
 * \brief Return TRUE if control key was pressed
*/

int ctrlIsPressed( void );



int getRepetition(void );



void setRepetition(int newRep);

/*
 * \fn int checkKeyboardActivity(void)
 *
 * \brief 	Return a 1 if the keyboard routine was called, so the keyboard
 * 			had interrupted. This funtcion is used by the screensaver, to turn
 * 			off.
 */

int checkKeyboardActivity(void);

/*
 * \fn void setKeyboardActivity(void)
 *
 * \brief Set the keyboard activity flag to 0.
 */

void setKeyboardActivity(void);

#endif
