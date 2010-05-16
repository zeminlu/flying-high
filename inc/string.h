/**
 *	\file string.h
 *
 *		\brief Declaration of all functions that deals with strings and buffers.
 *
 * 		\author Luciano Zemin, Nicolás Magni, Nicolás Purita	
 *
 */

#ifndef _STRING_H_
#define _STRING_H_

#include "types.h"
#include "defs.h"

/**
 * \fn char *strncpy(char *dst, const char *source, int n)
 *
 * 		\brief Brief.
 *
 * 		\param dst ParamBrief.
 * 		\param source ParamBrief.
 * 		\param n ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *strncpy(char *dst, const char *source, int n);

/**
 * \fn int stringHasChar(const char *string, const char c)
 *
 * 		\brief Brief.
 *
 * 		\param string ParamBrief.
 * 		\param c ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int stringHasChar(const char *string, const char c);

/**
 * \fn char *strstr(const char *haystack, const char *needle)
 *
 * 		\brief Brief.
 *
 * 		\param haystack ParamBrief.
 * 		\param needle ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *strstr(const char *haystack, const char *needle);

/**
 * \fn char *strcat(char *dst, const char *src)
 *
 * 		\brief Brief.
 *
 * 		\param dst ParamBrief.
 * 		\param src ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *strcat(char *dst, const char *src);

/**
 * \fn int strlen(const char *str)
 *
 * 		\brief Returns the lenght of the string.
 *
 * 		\param str ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int strlen(const char *str);

/**
 * \fn int strcmp(const char *s1, const char *s2)
 *
 * 		\brief The strcmp() function compares the two strings s1 and s2.  
 *				It returns an integer less than, equal to, or greater than zero if 
 * 				s1 is found, respectively, to be less than, to match, or be greater than s2.
 *
 * 		\param s1 ParamBrief.
 * 		\param s2 ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int strcmp(const char *s1, const char *s2);

/**
 * \fn void *memcpy(void *dest, const void *src, size_t n)
 *
 * 		\brief The memcpy() function copies n bytes from memory
 *				area src to memory area dest. The memory areas should not
 *				overlap.
 *
 * 		\param dest the destination memory address
 * 		\param src the source memory address
 * 		\param n the amount of bytes to be copied
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * \fn void *memset(void *dest, int c, size_t n)
 *
 * 		\brief The memset() function sets n bytes from the memory
 *				area dest with the value of c.
 *
 * 		\param dest ParamBrief.
 * 		\param c ParamBrief.
 * 		\param n ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void *memset(void *dest, int c, size_t n);

/**
 * \fn int strncmp(const char *s1, const char *s2, size_t n)
 *
 * 		\brief The strcmp() function compares the two strings s1 and s2 up to the nth character.  
 * 				It returns an integer less than, equal to, or greater than zero if 
 * 				s1 is found, respectively, to be less than, to match, or be greater than s2.
 *
 * 		\param s1 ParamBrief.
 * 		\param s2 ParamBrief.
 * 		\param n ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * \fn char *strcpy(char *dst, const char *source)
 *
 * 		\brief Brief.
 *
 * 		\param dst ParamBrief.
 * 		\param source ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *strcpy(char *dst, const char *source);

/**
 * \fn char *strtok (char * s, char * tokens)
 *
 * 		\brief Brief.
 *
 * 		\param s ParamBrief.
 * 		\param t ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *strtok (char * s, char * tokens);

/**
 * \fn int toInt(char * string)
 *
 * 		\brief Brief.
 *
 * 		\param string ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int toInt(char * string);

/**
 * \fn char *intToString(int num, char *str)
 *
 * 		\brief Brief.
 *
 * 		\param num ParamBrief.
 * 		\param str ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *intToString(int num, char *str);

/**
 * \fn char *doubleToString(double num, char *str)
 *
 * 		\brief Brief.
 *
 * 		\param num ParamBrief.
 * 		\param str ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *doubleToString(double num, char *str);

/**
 * \fn char *toLowerCase(char * string)
 *
 * 		\brief Brief.
 *
 * 		\param string ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
char *toLowerCase(char * string);

/**
 * \fn int isAllNumb(char *value)
 *
 * 		\brief Brief.
 *
 * 		\param value ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int isAllNumb(char *value);

#endif
