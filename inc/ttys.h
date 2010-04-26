/*
 * ttys.h
 */

#ifndef TTYS
#define TTYS

#include "keyboard_buffer.h"
#include "video_driver.h"
#include "string.h"

extern unsigned char videoBuffer[VIDEO_MEMORY_SIZE];

extern Keycode keyboardBuffer[KEYBOARD_BUFFER_SIZE];

void printToScreen(char * str, int amm);

void putsTTY( unsigned char *name, int count );

void putcharTTY( char c );

void initializeTTY( void );

int getCurrentTTY( void );

void setTtyFocusProcess( tty_t tty, pid_t pid );

int changeTtyFocus( tty_t nextTty );

#endif