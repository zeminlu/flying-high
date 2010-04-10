/*
 *  \file queue.c
 *  \author Nicolas Purita
 *
 *  \brief 	Make a circular buffer queue. The first time that the keyboard want to enqueue, this
 *  		buffer start to save in position 1 and then continue as normally. If the pointer where it
 *  		is adding is equal to the size of the queue start in the beginning of the buffer but at this
 *  		time it will enqueue to the position 0.
 */


#include "queue.h"


/*
 * Function Defines
 */


static int curr = 0, tail = 0;


int charEnque ( Keycode elem )
{
	if ( curr == KEYBOARD_BUFFER_SIZE - 1)
	{
		if( tail != 0 )
		{
			keyboardBuffer[curr] = elem;
			curr = 0;
		}else
			--curr;	
	}
	else
	{
		if( curr+1 == tail )
			--curr;
		else
			keyboardBuffer[curr++] = elem;
	}
	return TRUE;

}

Keycode charDeque ( void )
{
	 if( tail == KEYBOARD_BUFFER_SIZE )
	        tail = 0;
	 if( tail == curr )
		 return '\0';

	 return keyboardBuffer[tail++];
}


int kbBufferIsEmpty( void )
{
	return curr == tail;
}
