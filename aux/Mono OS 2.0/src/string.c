/*	string.c
*
*	Perform string operations on null-terminated 
*		sequences of characters
*	Author: Santiago Perez De Rosso
*/

/*
 *	Includes	
 *	========
 */
#include "stdio.h"
#include "string.h"
#include "sysfilesystem.h"
#include "debug.h"

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

/**
 *	@fn static int catToFixString(char *fixString, char *stringToAdd, int sizeOfFixString, int showErrorMessage)
 *	@brief	Appends the stringToAdd string to the fixString string, overwriting the null byte ('\0') at the end of fixString, 
 *		and then adds a terminating null byte if and only if the size of the fix string (sizeOfFixString) supports the 
 *		operation. If showErrorMessage is TRUE then an error message is printed on the standard output
 *		@param	fixString the string of destination
 *		@param	stringToAdd the source string
 *		@param	sizeOfFixString the size of the string of destination
 */
int
catToFixString(char *fixString, char *stringToAdd, int sizeOfFixString, int showErrorMessage){
	if(strlen(fixString)+strlen(stringToAdd) >= sizeOfFixString ){
		if(showErrorMessage)
			puts((unsigned char *) "The operation couldn't be realized, not enough space (the string couldn't be added) \n");	
		return FALSE;
	}
	strcat(fixString, stringToAdd);
	return TRUE;
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

/**
 *	@fn static int cpyToFixString(char *fixString, char *src, int sizeOfFixString, int showErrorMessage)
 *	@brief	Copies (if possible) the string pointed to by src, including the terminating null 
 *		byte ('\0'), to the buffer pointed to by fixString. The strings may not over‐lap. 
 *		The string is copied <==> there is enough room in fixString for the string src. If 
 *		showErrorMessage is TRUE then an error message is printed on the standard output
 *
 */
int
cpyToFixString(char *fixString, char *src, int sizeOfFixString, int showErrorMessage){
	if(strlen(fixString)+strlen(src) >= sizeOfFixString ){
		if(showErrorMessage)
			puts((unsigned char *) "The operation couldn't be realized, not enough space in the destination string \n");	
		return FALSE;
	}
	strcpy(fixString, src);
	return TRUE;

}
/*
void *memcpy(void *dest, const void *src, size_t n){
	char *d=( char *)dest;
	char *s=( char *)src;
	
	for(d=dest; d-( char *)dest < n; ++d)
		*d=*s++;
	return dest;
}
*/
/*
void* memmove( void * dest, const void * src, size_t n){
	char *temp;
	void * algo;
	puts("MEMOVEEEEEEE PAPA \n");
	puts(((path_t *)src)->file.fileName);
	int_debug(FILESYS_DEBUG, __FUNCTION__, n, TRUE);
	temp = tempCalloc(n, 1);
	memcpy(temp, src, n);
	puts(" \n");
	puts(((path_t *)temp)->file.fileName);
	algo = memcpy(dest, temp, n);
	
	puts(((path_t *)dest)->file.fileName);
	return dest;
}
*/
void *
memmove (void *dest, const void *src, size_t len){
	char *d = dest;
	const char *s = src;
	if (d < s)
		while (len--)
			*d++ = *s++;
	else {
		char *lasts = s + (len-1);
		char *lastd = d + (len-1);
		while (len--)
			*lastd-- = *lasts--;
	}
	return dest;
}

int
memcmp (const void *str1, const void *str2, size_t count){
	const unsigned char *s1 = str1;
	const unsigned char *s2 = str2;

	while (count-- > 0){
		if (*s1++ != *s2++)
			return s1[-1] < s2[-1] ? -1 : 1;
	}
	return 0;
}

void *
memcpy (void *dest, const void *src, size_t len){
	char *d = dest;
	const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}

void *
memset (void *dest, int val, size_t len){
	unsigned char *ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

/*
void *memset(void *dest, int c, size_t size) {
	unsigned char *buff = dest;
	int i;

	for ( i = 0; i < size; ++i )
		*buff++ = c;
	return dest;
}
*/
char * strtok (char * s, char * tokens)
{
	static char * string,  * ret, * ptr;
	int i, tokensLen, strLength;
	char * temp;

	if ( tokens == NULL || (s == NULL && string == NULL) )
		return NULL;
	else if ( s != NULL )
	{
		string = s;
		strLength = strlen(string);
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
		if ( ret == string && ptr == string + strLength )
			return NULL;
		temp = ret;
		ret = ptr;
		return temp;
	}
	return NULL;
}

int toInt(char * string )
{
	int num=0;

	while ( *string != '\0' && '0' <= *string && *string <= '9' )
		num = num * 10 + *string++ - '0';

	return num;
}

char *intToString(int num, char *str) {
	int i;

	if ( str == NULL )
		return NULL;
	
	i = 0;
	if ( num < 0 ) {
		num *= -1;
		str[0] = '-';
		str++;
	}
	return ritoa(num, str, &i);
}

char * toLowerCase(char * string )
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
