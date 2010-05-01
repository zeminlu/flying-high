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

extern process_t *runningProcess;

/*extern Keycode keyboardBuffer[KEYBOARD_BUFFER_SIZE];*/

void putsTTY( unsigned char *name, int count, tty_t tty );

void putCharTTY( char c, tty_t tty );

void initializeTTY( void );

int getCurrentTTY( void );

void setFocusProcessTTY( tty_t tty, pid_t pid );

int changeFocusTTY( tty_t nextTty );

void refreshTTY(void);

Keycode sysGetChar(tty_t tty);

void sysPutS(Keycode *name, int count,tty_t tty);

void sysPutChar(Keycode c, tty_t tty);

void sysSetFocusProcessTTY(pid_t pid, tty_t tty);

pid_t sysGetFocusProcessTTY(tty_t tty);


#endif