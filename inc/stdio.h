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

/*extern FILE **fileSystem;*/
extern process_t *runningProcess;

/*
 *	Standard I/O Files Pointers
 */
#define stdin (runningProcess->files[STDIN])
#define stdout (runningProcess->files[STDOUT])

/*
 *	Standard I/O Functions
 */
#define puts(str) fputs((str), stdout)
#define putchar(c) fputc((c), stdout)
#define getchar() fgetc(stdin)
#define puti(i) fputi((i), stdout)
#define putx(num) fputx((num), stdout)

/**
 * \fn int fputi(int i, FILE * stream)
 *
 * 		\brief Brief.
 *
 * 		\param i ParamBrief.
 * 		\param stream ParamBrief.
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
int fputi(int i, FILE * stream);

/**
 * \fn int fputx(int i, FILE * stream)
 *
 * 		\brief Brief.
 *
 * 		\param i ParamBrief.
 * 		\param stream ParamBrief.
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
int fputx(int i, FILE * stream);

/**
 * \fn int fputs ( const char *s, FILE * stream )
 *
 * 		\brief Brief.
 *
 * 		\param s ParamBrief.
 * 		\param stream ParamBrief.
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
int fputs ( const char *s, FILE * stream );

/**
 * \fn int fputc ( int c, FILE * stream )
 *
 * 		\brief Brief.
 *
 * 		\param c ParamBrief.
 * 		\param stream ParamBrief.
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
int fputc ( int c, FILE * stream );

/**
 * \fn char * fgets ( char * s, int size, FILE * stream )
 *
 * 		\brief Brief.
 *
 * 		\param s ParamBrief.
 * 		\param size ParamBrief.
 * 		\param stream ParamBrief.
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
char * fgets ( char * s, int size, FILE * stream );

/**
 * \fn int fgetc ( FILE * stream )
 *
 * 		\brief Brief.
 *
 * 		\param stream ParamBrief.
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
int fgetc ( FILE * stream );

/**
 * \fn void flush ( FILE * stream )
 *
 * 		\brief Brief.
 *
 * 		\param stream ParamBrief.
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
void flush ( FILE * stream );

/**
 * \fn int putsInStdTTY(const char  *s)
 *
 * 		\brief Brief.
 *
 * 		\param s ParamBrief.
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
int putsInStdTTY(const char  *s);

#endif
