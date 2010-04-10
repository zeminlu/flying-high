/*
 * 	File stdio.h
 *	
 * 		Brief
 *
 * 		Author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */
#include "stdio.h"
#include "system.h"
#include "string.h"
#include "unistd.h"

extern FILE fileSystem[];

FILE * stdoutFile = &(fileSystem[STDOUT]);
FILE * stdinFile = &(fileSystem[STDIN]);
FILE * inattFile = &(fileSystem[IN_ATT]);
FILE * outattFile = &(fileSystem[OUT_ATT]);

int fputc ( int c, FILE * stream )
{
	if ( stream == NULL  )
		return EOF;
	if ( write(stream->fd, &c, 1) != 1 )
		return EOF;
	return c;
}

int fgetc ( FILE * stream )
{
	int c;
	if ( stream == NULL )
		return EOF;
	if ( read(stream->fd, &c, 1) != 1 )
		return EOF;
	return c;
}

int fputs ( const char * s, FILE * stream )
{
	int ret;

	if ( s == NULL || stream == NULL )
		return EOF;
	if ( (ret = write(stream->fd, s, strlen(s))) == EOF )
		return EOF;
	return ret;
}

char * fgets ( char * s, int size, FILE * stream )
{
	if ( stream == NULL || s == NULL || size <= 0 )
		return NULL;
	if ( read(stream->fd, s, size) == EOF )
		return NULL;
	return s;
}

void flush ( FILE * stream )
{
	if ( stream == NULL )
		return;
	stream->ptr = stream->buffer;
}
