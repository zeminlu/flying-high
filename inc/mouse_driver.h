/*
 * \file mouse_driver.h
 *
 *	\author: Guido Marucci Blas & Nicolas Purita & Luciano Zemin
 */

#ifndef MOUSE_DRIVER_H_
#define MOUSE_DRIVER_H_

/**
 *	\fn void initMouse (void)
 *  	\brief This function initiates the mouse and leaves it working with the default values, through the IRQ12.
 *      
 *      \code
 *          mouseInit();
 *	\endcode
 */
void initMouse (void);

/**
 *	\fn void refreshMouse (void)
 *  	\brief This function checks weather the mouse informed a click or a movement, and refreshes it on the screen.
 *      
 *      \code
 *          mouseInit();
 *	\endcode
 * 	
 * 	\sa mouseDriver()
 */
void refreshMouse(void);

/**
 *	\fn void mouseDriver (void)
 *  	\brief This function is the one called by the INT 74h handler (IRQ12), and it collects the data sent by the mouse
 *		sticking to the PS/2 mouse protocol, and refreshes the values that later refreshMouse() will check.
 *      
 *      \code
 *          mouseDriver();
 *	\endcode
 * 
 * 	\sa refreshMouse()
 */
void mouseDriver (void);

/**
 *	\fn void setMouseRate (int rate)
 *  	\brief This function sets the speed of the mouse according to the rate given parameter.
 *      
 * 	\param rate The int parameter that upwards from 0, sets the speed of the mouse, decreasingly.
 *      
 * 	\code
 *          setMouseRate(10);
 *	\endcode
 * 	
 * 	\sa setMouseState()
 */
void setMouseRate (int rate);

/**
 *	\fn void setMouseState (int newState)
 *  	\brief This function enables or disables the mouse refreshing on the screen.
 *      
 *	\param newState The new state for the mouse. It can be wether ENABLED or DISABLED.
 *      
 * 	\code
 *          setMouseState(ENABLED);
 *	\endcode
 * 
 * 	\sa setMouseRate()
 */
void setMouseState (int newState);

/**
 *	\fn int checkMouseActivity (void)
 *  	\brief This function checks wether the mouse driver acknowledged mouse activity since the last call to this function.
 * 		Disabling the mouse through the setMouseState function doesn't modify the functionality of this function.
 *
 *	\return TRUE if there has been mouse activity since last call, FALSE if not.       
 *
 *      \code
 *          int activity = checkMouseActivity();
 *	\endcode
 * 
 * 	\sa setMouseState()
 */
int checkMouseActivity (void);

#endif
