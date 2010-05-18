/**
 *	\file queue.h
 *
 *		\brief Keyboard queue module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "keyboard_buffer.h"
#include "types.h"
/*
 * Define Section
 */

/*
 * Function Section
 */

/**
 * 	\fn void charEnque ( Keycode elem )
 *
 * 	\param elem	This function receive the ASCII code of the key to enqueue in the buffer.
 *
 * 	\return	TRUE if the key was added or FALSE if the buffer is FULL
 */

int charEnque ( Keycode elem );

/**
 * \fn keycode charDeque ( void )
 *
 * \brief Dequeue an element while the buffer is not Empty
 *
 * \return	The keycode
 */

Keycode charDeque ( void );

/**
 *	\fn int kbBufferIsEmpty ( void )
 *
 *	\brief return if the buffer is empty
 *
 *	\return TRUE if it's empty, FALSE if not
 */

int kbBufferIsEmpty ( void );

#endif