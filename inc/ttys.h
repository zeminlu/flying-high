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
#include "syscall.h"
#include "types.h"
#include "unistd.h"
#include "kMalloc.h"

void getReadPointer(tty_t tty, int *readPointer, int *readCol, int *readRow);

void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow);

void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow);

void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow);

void putsTTY( unsigned char *name, int count, tty_t tty );

void putCharTTY( char c, tty_t tty, int inStdIn );

void initializeTTY( void );

int getFocusedTTY( void );

int changeFocusTTY( tty_t nextTty );

void refreshKeyboardBufferTTY( void );

void refreshTTY(void);

Keycode sysGetChar(tty_t tty);

void sysPutS(Keycode *name, int count,tty_t tty);

void sysPutChar(Keycode c, tty_t tty);

void sysSetTTYFocusedProcess(pid_t pid, tty_t tty);

pid_t sysGetTTYFocusedProcess(tty_t tty);

void clearTTYScreen();

void setTTYCursorPosition( int x, int y);

void putTTYCharAtPosition( int c, int row, int col);

#endif