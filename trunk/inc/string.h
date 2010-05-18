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

/**
 * \fn char *strncpy(char *dst, const char *source, int n)
 *
 * 		\brief Copies the first n bytes from the source string to the dst string.
 *
 * 		\param dst The destination string.
 * 		\param source The source string.
 * 		\param n The ammount of bytes to be copied.
 * 		
 * 		\return A pointer to the copied string.
 *
 */
char *strncpy(char *dst, const char *source, int n);

/**
 * \fn int stringHasChar(const char *string, const char c)
 *
 * 		\brief Checks wether the given string contains or not the given character.
 *
 * 		\param string The string to be checked.
 * 		\param c The character to be checked.
 * 		
 * 		\return True if found, False if not.
 *
 */
int stringHasChar(const char *string, const char c);

/**
 * \fn char *strcat(char *dst, const char *src)
 *
 * 		\brief Concatenates the src string to the end of the dst string.
 *
 * 		\param dst The destination string.
 * 		\param src The source string.
 * 		
 * 		\return A pointer to the concatenated string.
 *
 */
char *strcat(char *dst, const char *src);

/**
 * \fn int strlen(const char *str)
 *
 * 		\brief Returns the length of the string.
 *
 * 		\param str The string whose length will be calculated.
 * 		
 * 		\return The length of the given string.
 *
 */
int strlen(const char *str);

/**
 * \fn int strcmp(const char *s1, const char *s2)
 *
 * 		\brief The strcmp() function compares the two strings s1 and s2.  
 *				
 * 		\param s1 The first string to be compared.
 * 		\param s2 The second string to be compared.
 * 		
 * 		\return It returns an integer less than, equal to, or greater than zero if 
 * 				s1 is found, respectively, to be less than, to match, or be greater than s2.
 *
 */
int strcmp(const char *s1, const char *s2);

/**
 * \fn void *memcpy(void *dest, const void *source, size_t n)
 *
 * 		\brief The memcpy() function copies n bytes from memory
 *				area src to memory area dest. The memory areas should not
 *				overlap.
 *
 * 		\param dest the destination memory address
 * 		\param source the source memory address
 * 		\param n the amount of bytes to be copied
 * 		
 * 		\return A pointer to the copied memory.
 *
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * \fn void *memset(void *dest, int val, size_t len)
 *
 * 		\brief The memset() function sets n bytes from the memory
 *				area dest with the value of c.
 *
 * 		\param dest Where the memory will be setted.
 * 		\param val The value with which the memory will be setted.
 * 		\param len The ammount of bytes to be setted.
 * 		
 * 		\return A pointer to the setted memory.
 *
 */
void *memset(void *dest, int c, size_t n);

/**
 * \fn int strncmp(const char *s1, const char *s2, size_t n)
 *
 * 		\brief The strcmp() function compares the two strings s1 and s2 up to the nth character.  
 *
 * 		\param s1 The first string to be compared.
 * 		\param s2 The second string to be compared.
 * 		\param n The amount of bytes to be taken into acount form each string.
 * 		
 * 		\return It returns an integer less than, equal to, or greater than zero if 
 * 				s1 is found, respectively, to be less than, to match, or be greater than s2.
 *
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * \fn char *strcpy(char *dst, const char *source)
 *
 * 		\brief Copies the source Null terminated string to the dst string.
 *
 * 		\param dst Where to copy the string.
 * 		\param source The Null terminated string to be copied.
 * 		
 * 		\return A pointer to the copied string.
 *
 */
char *strcpy(char *dst, const char *source);

/**
 * \fn char *strtok (char * s, char * tokens)
 *
 * 		\brief Gets segments of the given string delimitated by the given tokens.
 *
 * 		\param s The string to be segmentated.
 * 		\param tokens The tokens that limitate the segments.
 * 		
 * 		\return A pointer to the current segment.
 *
 */
char *strtok (char * s, char * tokens);

/**
 * \fn int toInt(char * string)
 *
 * 		\brief Transforms the given string to an int.
 *
 * 		\param string The string to be parsed.
 * 		
 * 		\return The int value of the string on success, -1 on error.
 *
 */
int toInt(char * string);

/**
 * \fn char *intToString(int num, char *str)
 *
 * 		\brief Transforms the given int number to a string.
 *
 * 		\param num The number to be transformed.
 * 		\param str A pointer where to write the string.
 * 		
 * 		\return The pointer to the transformed string on success, NULL on error.
 *
 */
char *intToString(int num, char *str);

/**
 * \fn char *doubleToString(double num, char *str)
 *
 * 		\brief Idem as intToString, but with double values.
 *
 * 		\param num The double number to be transformed.
 * 		\param str A pointer where to store the string.
 * 		
 * 		\return A pointer to the transformed string on success, NULL on error.
 *
 * 		\sa intToString()
 *
 */
char *doubleToString(double num, char *str);

/**
 * \fn char *toLowerCase(char * string)
 *
 * 		\brief Passes every UpperCase character from string to LowerCase.
 *
 * 		\param string The string to be lowercased.
 * 		
 * 		\return A pointer to the lowercased string on success, NULL on error.
 *
 */
char *toLowerCase(char * string);

/**
 * \fn int isAllNumb(char *value)
 *
 * 		\brief Checks wether the given string contains only numbers.
 *
 * 		\param value The string to be parsed.
 * 		
 * 		\return True if it's all numbers, False if not.
 *
 */
int isAllNumb(char *value);

#endif
