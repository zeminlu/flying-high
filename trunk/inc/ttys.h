/*
 * ttys.h
 */

#ifndef TTYS
#define TTYS

#include "keyboard_buffer.h"
#include "video_driver.h"

extern unsigned char videoBuffer[VIDEO_MEMORY_SIZE];

extern Keycode keyboardBuffer[KEYBOARD_BUFFER_SIZE];

void initializeTTY( void );

int getCurrentTty( void );

void setTtyFocusProcess( tty_t tty, pid_t pid );

int changeTtyFocus( tty_t nextTty );

static void KBCleaner ( void );

#endif