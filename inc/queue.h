/*
 *	\file queue.h
 *	\author Guido Marucci Blas, Luciano Zemin & Nicolas Purita
 *
 */
#include "keyboard_buffer.h"

/*
 * Define Section
 */


enum { FALSE, TRUE };


/*
 * Function Section
 */

/*
 * 	\fn void charEnque ( keycode elem )
 *
 * 	\param keycode 	This function receive the ASCII code of the key to enqueue in the buffer.
 *
 * 	\ret 			TRUE if the key was added or FALSE if the buffer is FULL
 */

int charEnque ( Keycode elem );

/*
 * \fn keycode charDeque ( void )
 *
 * \brief Dequeue an element while the buffer is not Empty
 *
 * \ret		The keycode
 */

Keycode charDeque ( void );

/*
 *	\fn int kbBufferIsEmpty ( void )
 *
 *	\brief return if the buffer is empty
 *
 *	\ret TRUE if it's empty, FALSE if not
 */

int kbBufferIsEmpty ( void );

