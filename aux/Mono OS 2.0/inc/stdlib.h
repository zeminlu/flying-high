/*
*	Standard Library
*
*	Author: Santiago Perez De Rosso
*/

#ifndef STDLIB_H_
#define STDLIB_H_

#include "stdio.h"

/*
*	Macro: isdigit(x)
*	Description: returns true if x is a digit o false if otherwise
*/
#define isdigit(x)	(x>='0' && x<='9')
/*
*	Macro: isspace(x)
*	Description: returns true if x is a space or tab or false if 
*	otherwise
*/
#define isspace(x)	(x==' ' || x=='\t')

/*   
*	Function: void *calloc(size_t nmemb, size_t size)
*	Description: Allocates memory for an array of nmemb elements of size 
*	bytes each and returns a pointer to the allocated memory.  The 
*	memory is set to zero.  If nmemb or size is 0, then calloc() 
*	returns NULL.
*/
//void *calloc(size_t nmemb, size_t size);

/*
*
*	Function: void *malloc(size_t size)
*	Description: Allocates size bytes and returns a pointer to the allocated 
*	memory.  The memory is not cleared. If size is 0, then malloc() 
*	returns NULL.
*/
//void *malloc(size_t size);

/*
*	Function: void free(void *ptr);
*	Description: Frees  the  memory  space pointed to by ptr, which must 
*	have been returned by a previous call to malloc(), calloc() or 
*	realloc().  Otherwise, or if free(ptr) has already been called before,
*	undefined behavior occurs.  If ptr is NULL, no operation is performed.
*	Frees all allocated memory from the pointer onwards
*
*/
//void free(void *ptr);

//void *realloc(void *ptr, size_t size);

/*
*	// Taken from K&R  (page 80 - spanish edition) //
*	Function: double atof(const unsigned char *nptr)
*	Description: Converts the string nptr to double 
*	Parameters:
*		nptr: the string to convert to double 
*	Returns:
*		a double that equals the given string
*/
double atof(const unsigned char *nptr);

/*
*	Function: int atoi(const unsigned char *nptr)
*	Description: Converts the string nptr to integer
*	Parameters:
*		nptr: the string to convert to int 
*	Returns:
*		an integer that equals the given string
*/	
int atoi(const char *nptr);

#endif
