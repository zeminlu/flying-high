/**
 *	\file int_handlers.h
 *
 *		\brief Declarations of all interrupt handlers
 *
 *		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */

#ifndef INT_HANDLERS_H_
#define INT_HANDLERS_H_

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

void _int_74_handler ( void );

void _int_09_handler ( void );

#endif
