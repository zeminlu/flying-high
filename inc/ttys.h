/*
 * ttys.h
 */

#ifndef TTYS
#define TTYS

#include "queue.h"
#include "video_driver.h"
#include "string.h"
#include "keyboard_buffer.h"
#include "sysProcess.h"

extern unsigned char videoBuffer[VIDEO_MEMORY_SIZE];

extern process_t processTable[MAX_PROCESS];

/*extern Keycode keyboardBuffer[KEYBOARD_BUFFER_SIZE];*/

void printToScreen(char * str, int amm);

void putsTTY( unsigned char *name, int count, tty_t tty );

void putcharTTY( char c, tty_t tty );

void initializeTTY( void );

int getCurrentTTY( void );

void setTtyFocusProcess( tty_t tty, pid_t pid );

int changeTtyFocus( tty_t nextTty );

#endif