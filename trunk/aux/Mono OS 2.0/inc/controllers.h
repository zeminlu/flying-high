/*	
*	File:controllers.h
*	Author: Santiago Perez De Rosso
*
*/
#ifndef CONTROLLERS_H_
#define CONTROLLERS_H_

/*
* Public functions
* ===============
*
*/
/*
*	Function: ssize_t writeController(int fd, const void *buf, size_t count)
*	Description: Writes on the fd given, count bytes of the buffer buf
*	Return:
*		the amount of bytes written
*/
ssize_t writeController(int fd, const void *buf, size_t count);

/*
*	Function: ssize_t readController(int fd, void *buf, size_t count)
*	Description: Reads on the buffer count bytes from the fd given
*	Return:
*		the amount of bytes read 
*/
ssize_t readController(int fd, void *buf, size_t count);

#endif
