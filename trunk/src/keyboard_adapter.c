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
#include "ttys.h"

void flushKeyboardBuffer( void )
{
	Keycode deChar = 0;
	int color;
	
	color = getVideoColor();

	while( !kbBufferIsEmpty() )
		if( (deChar = charDeque()) != '\0' )
		{
			if( (deChar & 0x80 ) == 1 )
				changeTtyFocus( (deChar & 0x81) - 1 );
			else
			{
				fputc((int)deChar, stdin );
				fputc(color, inatt);
			}
		}

}

