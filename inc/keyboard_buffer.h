/*
 *	\file: keyboard_buffer.h
 *
 *	\brief	This is the internal keyboard buffer.
 */
#ifndef _KEYBOARD_BUFFER_H_
#define _KEYBOARD_BUFFER_H_

#define KEYBOARD_BUFFER_SIZE 256

typedef unsigned char Keycode;

extern Keycode keyboardBuffer[];

#endif