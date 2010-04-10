/*
 *	File string.c
 *
 *		Brief: Implementation of string.h. String releated functions.
 *
 * 		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */
#include "string.h"


void * memcpy(void * dest, const void * source, size_t n)
{
	size_t i;

	if ( dest == NULL || source == NULL )
		return NULL;

	for ( i = 0; i < n; ++i ) 
		((char *)dest)[i] = ((char *)source)[i];
	
	return dest;
}

int strlen ( const char * str )
{
	int counter = 0;

	if ( str == NULL )
		return -1;
	
	while ( *str++ != '\0' )
		++counter;
	return counter;
}

char * strtok ( char * s, char * tokens )
{
	static char * string,  * ret, * ptr;
	int i, tokensLen;
	char * temp;

	if ( tokens == NULL || (s == NULL && string == NULL) )
		return NULL;
	else if ( s != NULL )
	{
		string = s;
		ptr = string;
	}

	ret = ptr;
	tokensLen = strlen(tokens);
	while ( *ptr != '\0' )
	{
		for ( i = 0; i < tokensLen; ++i )
			if ( (*ptr == tokens[i]) )
			{
				if ( *ret != *ptr )
				{
					*ptr++ = '\0';
					return ret;
				}
				else
					ret = ptr + 1;
			}
		++ptr;	
	}
	if ( ret != ptr )
	{
		temp = ret;
		ret = ptr;
		return temp;
	}
	return NULL;
}

int strcmp ( char * a, char * b )
{
	for (; *a && *b; ++a, ++b )
		if ( *a == *b )
			continue;
		else
			return *a - *b;
	
	return *a - *b;
}

char * strcpy ( char * dest, char * source )
{
	char * ret = dest;
	while (  (*dest++ = *source++) != '\0' );
	return ret;
}

int toInt( char * string )
{
	int num=0;

	while ( *string != '\0' && '0' <= *string && *string <= '9' )
		num = num * 10 + *string++ - '0';

	return num;
}

char * toLowerCase( char * string )
{
	char *aux = string;

	while (	*string != '\0' )
	{
		if ( 'A' <= *string && *string <= 'Z' )
			*string += 0x20;
		++string;
	}
	return aux;
}

int isAllNumb(char *value)
{
	while ( *value != '\0'  )
	{
		if ( '0' <= *value && *value <= '9' )
			++value;
		else
			return 0;
	}
	return 1;
}
