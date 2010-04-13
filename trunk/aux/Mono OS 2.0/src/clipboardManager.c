/*
*
*	File: clipboardManager
*	Author: Santiago Bermudez, Lucas Pizzagalli, Santiago Perez De Rosso
*
*	Provides functions for manipulating the clipboard	
*/

/*
*	Includes
*/
#include "stdlib.h"
#include "stdio.h"
#include "controllers.h"
#include "videoDriver.h"
#include "KBDriver.h"

/*
*	Defines
*/
#define NULL ((void *)0)

static size_t size;
static unsigned char clipBuffer[10000];

void
copy(int xInf, int xSup, int yInf, int ySup){
	size=(xSup-xInf+3)/2*(ySup-yInf+1);
	bufferizeScreen(xInf, xSup, yInf, ySup, clipBuffer);
	return;
}

void
paste(){
	videoDriverWrite(stdout, clipBuffer, size);
	pasteInKB(clipBuffer, size);
	return;
}
