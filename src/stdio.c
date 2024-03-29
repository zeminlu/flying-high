/**
 *	\file stdio.c
 *
 *		\brief Definitions for all standard input and output and
 * 		declaration of all functions to send/recive data to/from
 *		standar input/output.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "stdio.h"

#define NIBBLES_PER_INT sizeof(int) * 2

int fputi(int i, FILE * stream) {
	char msg[10];

	memset(msg, '\0', 10);
	intToString(i, msg);

	return fputs((const char *)msg, stream);
}

int fputx(int num, FILE * stream) {
	char msg[NIBBLES_PER_INT + 3];
	int i, nibbles, mask, c;

	memset(msg, '\0', NIBBLES_PER_INT + 3);
	mask = 0x0000000F;
	msg[0] = '0';
	msg[1] = 'x';
	for ( i = 0, nibbles = NIBBLES_PER_INT; i < nibbles; ++i ) {
		c = num & mask;
		if ( c < 0xA )	
			msg[NIBBLES_PER_INT - i + 1] = '0' + c;
		else
			msg[NIBBLES_PER_INT - i + 1] = 'A' + c - 10;
		num = num >> 4;
	}
	return fputs((const char *)msg, stream);
}

int fputc ( int c, FILE * stream )
{
	if ( stream == NULL  )
		return EOF;
	if ( fwrite(stream, &c, 1) != 1 )
		return EOF;
	return c;
}

int fgetc ( FILE * stream )
{
	int c;
	
	if ( stream == NULL )
		return EOF;
	if(fread(stream, &c, 1) != 1 ){
		return EOF;
	}
		
	return c;
}

int fputs ( const char * s, FILE * stream )
{
	int ret;

	if ( s == NULL || stream == NULL )
		return EOF;
	if ( (ret = fwrite(stream, (char *)s, strlen(s))) == EOF )
		return EOF;
	return ret;
}

char * fgets ( char * s, int size, FILE * stream )
{
	if ( stream == NULL || s == NULL || size <= 0 )
		return NULL;
	if ( fread(stream, s, size) == EOF )
		return NULL;
	return s;
}

void flush ( FILE * stream )
{
	if ( stream == NULL )
		return;
	stream->ptr = stream->buffer;
}