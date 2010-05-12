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


extern unsigned char videoBuffer[VIDEO_MEMORY_SIZE];

extern process_t processTable[MAX_PROCESS];

extern process_t *runningProcess;

extern process_t *initProcess;

extern Keycode *keyboardBuffer;

void getReadPointer(tty_t tty, int *readPointer, int *readCol, int *readRow);

void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow);

void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow);

void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow);

void putsTTY( unsigned char *name, int count, tty_t tty );

void putCharTTY( char c, tty_t tty, int inStdIn );

void initializeTTY( void );

int getFocusedTTY( void );

int changeFocusTTY( tty_t nextTty );

void refreshTTY(void);

Keycode sysGetChar(tty_t tty);

void sysPutS(Keycode *name, int count,tty_t tty);

void sysPutChar(Keycode c, tty_t tty);

void sysSetTTYFocusedProcess(pid_t pid, tty_t tty);

pid_t sysGetTTYFocusedProcess(tty_t tty);

void clearTTYScreen( tty_t tty );

void setTTYCursorPosition( int x, int y, tty_t tty);

void putCharATTTYPosition( int c, int row, int col, tty_t tty);

#endif