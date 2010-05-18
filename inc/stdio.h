/**
 *	\file stdio.h
 *
 * 		\brief Definitions for all standard input and output and
 * 		declaration of all functions to send/recive data to/from
 *		standar input/output.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita	
 *
 */

#ifndef _STDIO_H_
#define	_STDIO_H_

#include "string.h"
#include "unistd.h"
#include "types.h"

#define EOF	-1

extern process_t *runningProcess;

/*
 *	Standard I/O Files Pointers
 */

/**
 * \def stdin
 *		\brief stdin stream define.
 */
#define stdin (runningProcess->files[STDIN])
/**
 * \def stdout
 *		\brief stdout stream define.
 */
#define stdout (runningProcess->files[STDOUT])

/*
 *	Standard I/O Functions
 */

/**
 * \def puts(str)
 *		\brief puts function define.
 */
#define puts(str) fputs((str), stdout)
/**
 * \def putchar(c)
 *		\brief putchar function define.
 */
#define putchar(c) fputc((c), stdout)
/**
 * \def getchar()
 *		\brief getchar function define.
 */
#define getchar() fgetc(stdin)
/**
 * \def puti(i)
 *		\brief puti function define.
 */
#define puti(i) fputi((i), stdout)
/**
 * \def putx(num)
 *		\brief putx function define.
 */
#define putx(num) fputx((num), stdout)

/**
 * \fn int fputi(int i, FILE * stream)
 *
 * 		\brief Writes the given integer to the given stream.
 *
 * 		\param i The integer to write.
 * 		\param stream The stream where to write to.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 */
int fputi(int i, FILE * stream);

/**
 * \fn int fputx(int num, FILE * stream)
 *
 * 		\brief Writes the given integer as in hexadecimal format into the given stream.
 *
 * 		\param num The number to bw written.
 * 		\param stream The stream where to write the number to.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 */
int fputx(int i, FILE * stream);

/**
 * \fn int fputs ( const char *s, FILE * stream )
 *
 * 		\brief Writes the given string to the given stream.
 *
 * 		\param s A null terminate string to be written.
 * 		\param stream The stream where to write the string.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		\sa fgets()
 *
 */
int fputs ( const char *s, FILE * stream );

/**
 * \fn int fputc ( int c, FILE * stream )
 *
 * 		\brief Writes the given character to the given stream.
 *
 * 		\param c The character ascii to be written.
 * 		\param stream The stream where to write the character to.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		\sa fgetc()
 *
 */
int fputc ( int c, FILE * stream );

/**
 * \fn char * fgets ( char * s, int size, FILE * stream )
 *
 * 		\brief Reads a string of the given size from the given stream.
 *
 * 		\param s A pointer to the buffer where to write the string to.
 * 		\param size The ammount of bytes to be read.
 * 		\param stream The stream where to read the string from.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 * 		\sa fputs()
 *
 */
char * fgets ( char * s, int size, FILE * stream );

/**
 * \fn int fgetc ( FILE * stream )
 *
 * 		\brief Reads a character ascii from the given stream and returns it.
 *
 * 		\param stream The stream where to read the character from.
 * 		
 * 		\return The ascii from the read character.
 *
 * 		\sa fputc()
 *
 */
int fgetc ( FILE * stream );

/**
 * \fn void flush ( FILE * stream )
 *
 * 		\brief Flushes the given stream.
 *
 * 		\param stream The stream to be flushed.
 * 		
 */
void flush ( FILE * stream );

#endif
