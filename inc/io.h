/**
 * \file io.h
 * 	
 * 		\brief Declaration of functions and constants related with direct access
 * 		to hardware
 *
 * 		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */

#ifndef INOUT_H_
#define INOUT_H_

/*
 *	Video Memory Address
 */
#define VIDEO_MEMORY_ADDRESS (void *)0xB8000

/*
 *	Port Definitions
 */

/**
 *	\fn int inportb (int port)
 *		\brief This function grabs the value stored in the given port
 *
 *		\param port The desired port number.
 *		\return This function returns an int instance with the value obtained.
 *
 *		\code
 *			int value;
 *			value = inport(0x60);
 *		\endcode
 *
 *		\sa outport()
 *
 */
int inportb (int port);

/**
 * 	\fn void outportb (int port, int value)
 * 		\brief This function stores the given value in the given port.
 *
 * 		\param port Desired port number.
 * 		\param value Desired value to set.
 *
 * 		\code
 *			outport (0x64, 0x31)
 * 		\endcode
 *
 * 		\sa inport()
 *
 */
void outportb (int port, int value);

#endif
