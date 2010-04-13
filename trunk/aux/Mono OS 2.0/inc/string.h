/*	string.h
*
*	Perform string operations on null-terminated 
*		sequences of characters
*	Author: Santiago Perez De Rosso
*/

#ifndef STRING_H_
#define STRING_H_

/*
* Public functions
* ===============
*/
#ifndef stdioTypes
#define stdioTypes
typedef struct file_tCDT *file_t;
typedef long size_t;
typedef long ssize_t;
typedef int fDescriptor;
#endif


char *strncpy(char *dst, const char *source, int n);

int stringHasChar(const char *string, const char c);

char *strstr(const char *haystack, const char *needle);

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
catToFixString(char *fixString, char *stringToAdd, int sizeOfFixString, int showErrorMessage);
	

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
 *
 *
 *
 */

void* memmove( void * dest, const void * src, size_t n);
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

/**
 *	@fn static int cpyToFixString(char *fixString, char *src, int sizeOfFixString, int showErrorMessage)
 *	@brief	Copies (if possible) the string pointed to by src, including the terminating null 
 *		byte ('\0'), to the buffer pointed to by fixString. The strings may not over‐lap. 
 *		The string is copied <==> there is enough room in fixString for the string src. If 
 *		showErrorMessage is TRUE then an error message is printed on the standard output
 *
 */
int cpyToFixString(char *fixString, char *src, int sizeOfFixString, int showErrorMessage);
	
char *strcpy(char *dst, const char *source);

char *strtok (char * s, char * tokens);

int toInt(char * string);

char *intToString(int num, char *str);

char *toLowerCase(char * string);


int isAllNumb(char *value);

#endif
