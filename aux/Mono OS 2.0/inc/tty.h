/**
 *
 *		tty.h
 *
 *
 */

#ifndef TTY_H_
#define TTY_H_

#include "process.h"
#include "stdio.h"
#define MAXAMOUNTOFTTYS 6
#define CHANGETTYMASK 0x80

int initializeTTY();

int changeTTY(tty_t next);

int readFromTTY(unsigned char* buf,int  count);

int setTtyFocusProcess( tty_t tty, pid_t process);

int ttyBackSpaceOnScreen();

void ttyPutchar(unsigned char c);

tty_t getFocusTTY();

int  writeToTTY(int fd, const void *buf, size_t count);
#endif
