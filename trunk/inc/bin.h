#ifndef _BIN_H_
#define _BIN_H_

#include "sysProcess.h"
#include "stdio.h"
#include "shell.h"
#include "string.h"
#include "process.h"

void init(void *args);

void top(char *args);

void welcome();

void goodbye();

void printA();

void printB();

void nothing();

void pageFault();

#endif