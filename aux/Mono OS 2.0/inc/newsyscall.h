/**
 *	File newsyscall.h
 *
 * 		Brief: The implementation of all system calls
 *
 * 		Author: Santiago Bermudez
 */

#define GENERAL_WRITE_ERROR 	-1
#define FD_WRITE_ERROR			-2
#define RESIZE_WRITE_ERROR		-3

size_t _sys_write ( int fd, char * buffer, size_t n );
size_t _sys_read ( int fd, char * buffer, size_t n );
