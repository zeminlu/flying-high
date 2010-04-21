#ifndef _KEYBOARD_ADAPTER_H
#define _KEYBOARD_ADAPTER_H

#include "stdio.h"
#include "video_driver.h"
#include "queue.h"

/**
 * \fn void flushKeyboardBuffer(void)
 *
 * 	\brief This function obtain the character from the keyboard buffer, and
 * 	it puts it in STDIN.
 *
 */
void flushKeyboardBuffer(void);

#endif
