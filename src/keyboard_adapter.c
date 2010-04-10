/*
 *	\file keyboard_adapter.c
 *
 *	\brief 	Connect Stdin with the Keyboard buffer with it's
 *			respective attributes.
 *
 *	\code
 *		keyboard_adapter();
 *	\endcode
 */

#include "keyboard_adapter.h"
#include "stdio.h"
#include "video_driver.h"
#include "queue.h"



void flushKeyboardBuffer( void )
{
	Keycode deChar = 0;
	int color;
	
	color = getVideoColor();

	while( !kbBufferIsEmpty() )
		if( (deChar = charDeque()) != '\0' )
		{
			fputc((int)deChar, stdin );
			fputc(color, inatt);
		}

}

