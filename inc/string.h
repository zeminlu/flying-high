/**
 *	\file string.h
 *
 *		\brief Declaration of all functions that deals with strings and buffers.
 *
 * 		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin	
 *
 */

#ifndef _STRING_H_
#define _STRING_H_

#include "types.h"
#include "defs.h"
#include "stdio.h"

char *strncpy(char *dst, const char *source, int n);

int stringHasChar(const char *string, const char c);

char *strstr(const char *haystack, const char *needle);

char *strcat(char *dst, const char *src);

/*
* Function: int strlen(char *str)
* Description:Returns the lenght of the string
*	
*/
int strlen(const char *str);

/*
* Function: int strcmp(const unsigned char *s1, const unsigned char *s2)
* Description:  The strcmp() function compares the two strings s1 and s2.  
* It returns an integer less than, equal to, or greater than zero if 
* s1 is found, respectively, to be less than, to match, or be greater than s2.
*/
int strcmp(const char *s1, const char *s2);

/*
*	Function: memcpy
*	Description: The memcpy() function copies n bytes from memory
*	area src to memory area dest. The memory areas should not
*	overlap.
*	Parameters:
*		dest: the destination memory address
*		src: the source memory address
*		n: the amount of bytes to be copied
*/

void *memcpy(void *dest, const void *src, size_t n);

/*
 *	Function: memset
 *	Description: The memset() function sets n bytes from the memory
 *	area dest with the value of c.
 *
 */
void *memset(void *dest, int c, size_t n);

/*
* Function: int strncmp(const unsigned char *s1, const unsigned char *s2, size_t n)
* Description:  The strcmp() function compares the two strings s1 and s2 up to the nth character.  
* It returns an integer less than, equal to, or greater than zero if 
* s1 is found, respectively, to be less than, to match, or be greater than s2.
*/
int strncmp(const char *s1, const char *s2, size_t n);

char *strcpy(char *dst, const char *source);

char *strtok (char * s, char * tokens);

int toInt(char * string);

char *intToString(int num, char *str);

char *toLowerCase(char * string);


int isAllNumb(char *value);
#endif
