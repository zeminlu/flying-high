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

int fputi(int i, FILE * stream);

int fputx(int i, FILE * stream);

int fputs ( const char *s, FILE * stream );

int fputc ( int c, FILE * stream );

char * fgets ( char * s, int size, FILE * stream );

int fgetc ( FILE * stream );

void flush ( FILE * strema );

int putsInStdTTY(const char  *s);

#endif
