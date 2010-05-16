/**
 *	\file int_handlers.h
 *
 *		\brief Declarations of all interrupt handlers
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 */

#ifndef _INT_HANDLERS_H_
#define _INT_HANDLERS_H_

/**
 *	\fn void _int_08_handler ( void )
 *
 * 		\brief This function handles the timer tick 
 * 		interrupt
 *
 */
void _int_08_handler ( void );

/**
 *	\fn void _int_80_handler ( void );
 *		
 *		\brief This function handles the kernel system calls.
 *		Posible system calls:
 *			_SYS_WRITE
 *			_SYS_READ
 *		This rutine must be load into the IDT in the position 0x80
 *
 */
void _int_80_handler ( void );

/**
 * \fn void _int_09_handler ( void )
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void _int_09_handler ( void );

#endif
