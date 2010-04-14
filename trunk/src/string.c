/*
 *	File string.c
 *
 *		Brief: Implementation of string.h. String releated functions.
 *
 * 		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */
#include "string.h"

static char *ritoa(int num, char *str, int *i) {
	if ( (num / 10) == 0 ) {
		str[*i] = '0' + (num % 10);
		*i = (*i) + 1;
		return str;
	}
	ritoa(num / 10, str, i);
	str[*i] = '0' + (num % 10);
	*i = (*i) + 1;
	return str;
}

/*
* Public functions
* ================
*/

int stringHasChar(const char *string, const char c){
    int i, len=strlen(string);
    for(i=0; i<len; ++i)
	   if(string[i] == c)
		  return TRUE;
    return FALSE;
}

char *strcat(char *dst, const char *src){
	int i, j;
	for(i=0; dst[i] != '\0'; ++i);
	for(j=0; src[j] != '\0'; ++j, ++i)
		dst[i]=src[j];
	dst[i]='\0';
	return dst;
}

int strlen(const char *str){
	int ret;
	for(ret=0; str[ret] != '\0'; ++ret);
	return ret;
} 

int strcmp(const char *s1, const char *s2){
	const char *p=s1;
	const char *q=s2;
	if( s1 == s2 )
		return 0;	
	if( s1 == NULL )
		return -1;
	if( s2 == NULL )
		return 1;
	for( ; *p != '\0' && *q != '\0' && *p == *q; ++p, ++q);
	return *p-*q;
}

int strncmp(const char *s1, const char *s2, size_t n){
	const char *p=s1;
	const char *q=s2;
	int i=0;
	for( ; *p != '\0' && *q != '\0' && *p == *q && i<n; ++p, ++q, ++i);
	if( i == n )
		return 0;
	return *p-*q;
}

char *strncpy(char *dst, const char *source, int n){
	char *p=dst;
	const char *q=source;
	for( ; *q != '\0' && p-source < n; ++p, ++q)
		*p=*q;
	*p='\0';
	return dst;
}

char *strcpy(char *dst, const char *source){
	char *p=dst;
	const char *q=source;
	for( ; *q != '\0'; ++p, ++q)
		*p=*q;
	*p='\0';
	return dst;
}

/**	@fn  char *strstr(const char *haystack, const char *needle)
 * 		@brief The strstr() function finds the first occurrence of the substring needle in the string haystack.  The terminating '\0' characters are not compared.
 * 		@return These functions return a pointer to the beginning of the substring, or NULL if the substring is not found.
 */
char *strstr(const char *haystack, const char *needle){
   /* hay algoritmos mas eficientes para esto (lo vi en eda) pero con esto alcanza */
    int len=strlen(haystack), i;
    for(i=0; i<len; ++i){
	   if(strncmp(haystack+i, needle, strlen(needle)) == 0)
		  return ((char *) haystack + i);
    }
    return NULL;
}

int memcmp (const void *str1, const void *str2, size_t count){
	const unsigned char *s1 = str1;
	const unsigned char *s2 = str2;

	while (count-- > 0){
		if (*s1++ != *s2++)
			return s1[-1] < s2[-1] ? -1 : 1;
	}
	return 0;
}

void * memset (void *dest, int val, size_t len){
	unsigned char *ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

void * memcpy(void * dest, const void * source, size_t n)
{
	size_t i;

	if ( dest == NULL || source == NULL )
		return NULL;

	for ( i = 0; i < n; ++i ) 
		((char *)dest)[i] = ((char *)source)[i];
	
	return dest;
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
