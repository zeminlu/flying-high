/**
 *	\file unistd.h
 *
 *		\brief Definitions for all UNIX Standard system calls functions
 *		
 *		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */

#ifndef _UNISTD_H_
#define _UNISTD_H_

#include "types.h"

/*
 *	System Calls Functions
 */

/**
 *	\fn ssize_t write ( int fileds, const void * buffer, size_t count )
 *
 * 		\brief write() writes "count" bytes from the buffer pointed by "buffer"
 * 		into the file descripted by the file descriptor "fileds".
 *
 * 		\param fileds the file descriptor for the file where the data is going
 * 		to be written.
 * 		\param buffer pointer to the buffer where the information to be written
 * 		is hold.
 * 		\param count number of bytes that will be written in the file from the
 * 		buffer.
 *
 * 		\return the amount of bytes successfully written in the file or -1 in 
 * 		case of error.
 *
 * 		Example:
 * 			\code
 * 				char * message = "Hello World!\n";
 *				write(STDOUT, (const void *)message, strlen(message));
 * 			\endcode
 *
 * 		\sa read()
 *
 */
ssize_t write ( int fileds, const void * buffer, size_t count );

/**
 *	\fn ssize_t read ( int fileds, void * buffer, size_t count );
 *
 *		\brief read() reads "count" bytes from the file descripted by
 *		the file descriptor "fileds" and saves it into the buffer 
 *		pointed by "buffer"
 *		
 *		\param fileds the file descriptor for the file where the data is going
 * 		to be read.
 * 		\param buffer pointer to the buffer where the information is going to
 * 		written.
 * 		\param count number of bytes that will be written in the buffeer from the
 * 		file.
 *
 * 		\return the amount of bytes that were successfully read from the file
 * 		or -1 in case of error.
 *
 * 		Example
 * 			\code
 *				if ( read(STDIN, buffer, VIDEO_SIZE) != VIDEO_SIZE )
 *					printf("Error: video memory could not be read\n");
 * 			\endcode
 *
 * 		\sa write()
 *
 */
ssize_t read ( int fileds, void * buffer, size_t count );

#endif
