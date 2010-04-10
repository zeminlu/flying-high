/**
 *	\file stdio.h
 *
 * 		\brief Definitions for all standard input and output and
 * 		declaration of all functions to send/recive data to/from
 *		standar input/output.
 *
 *		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin	
 *
 */

#ifndef STDIO_H_
#define	STDIO_H_

#include "file.h"

#define EOF	-1
extern FILE * stdinFile;
extern FILE * stdoutFile;
extern FILE * inattFile;
extern FILE * outattFile;

/*
 *	Standard I/O Files Pointers
 */
#define stdin (stdinFile)
#define stdout (stdoutFile)
#define inatt (inattFile)


/*
 *	Standard I/O Functions
 */
#define puts(str) fputs((str), stdout)
#define putchar(c) fputc((c), stdout)
#define getchar() fgetc(stdin)

int fputs ( const char *s, FILE * stream );

int fputc ( int c, FILE * stream );

char * fgets ( char * s, int size, FILE * stream );

int fgetc ( FILE * stream );

void flush ( FILE * strema );

#endif
