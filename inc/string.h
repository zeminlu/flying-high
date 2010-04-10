/**
 *	\file string.h
 *
 *		\brief Declaration of all functions that deals with strings and buffers.
 *
 * 		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin	
 *
 */

#ifndef STRING_H_
#define STRING_H_

#include "types.h"

void * memcpy(void * dest, const void * source, size_t n); 

int strlen ( const char * str );

char * strtok ( char * s, char * tokens );

int strcmp ( char * s1, char * s2 );

char * strcpy ( char * dest, char * source );

int toInt ( char * string );

char * toLowerCase ( char * string );

int isAllNumb(char *value);
#endif
