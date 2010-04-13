/*	
*	File: controllers.c
*	Author: Santiago Perez De Rosso
*
*/

/*
* Includes
*
*/
#include "stdio.h"
#include "videoDriver.h"
#include "kasm.h"
#include "primitives.h"
#include "KBDriver.h"
#include "tty.h"

/*
* Public functions
* ================
*
*/
ssize_t 
writeController(int fd, const void *buf, size_t count){
/*	int i;
	if( fd == stdout ){
		return writeToTTY(fd, buf, count); 
	}*/
	return write(fd, buf, count);
}

ssize_t
readController(int fd, void *buf, size_t count){
/*	if( fd == stdin ){
		puts("Read controller--> SE ENTRO");
//		return readKBString(buf, count);
		return readFromTTY(buf, count);
	}
	puts("Read controller--> NOO SE ENTRO");
*/	return read(fd, buf, count);
}
