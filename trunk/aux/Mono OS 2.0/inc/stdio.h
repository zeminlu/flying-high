/*	stdio.h
*
*	Standard Input/Output library functions
*	Author: Santiago Perez De Rosso
*
*/

#ifndef STDIO_H_
#define STDIO_H_
/*
* Defines
* =======
*
*/
#ifndef NULL
#define NULL ((void *)0)
#endif
#ifndef stdin 
#define stdin 0
#define stdout 1
#define stdbk 2
#define stdtty 3
//#define screenBufferFD 1
#endif

#ifndef TRUE 
#define TRUE 1
#define FALSE 0
#define EOF 255 
#endif

#ifndef stdioTypes
#define stdioTypes
typedef struct file_tCDT *file_t;
typedef long size_t;
typedef long ssize_t;
typedef int fDescriptor;
#endif
/*
* Public functions
* ================
*
*/
/*
*
* Function: int putc(int c, file_t stream)
* Description:Writes the character c, to stream
*
*/
int putc(int c, fDescriptor stream);

/*
*
* Function: int fputs(const char *s, file_t stream)
* Description: Writes the string s, to stream (without the trailing '\0')
*
*/
int fputs(const unsigned char *s, fDescriptor stream);

/*
*
* Function: int putchar(int c)
* Description: Writes the character c, to the standard output	
*
*/
int putchar(int c);

/*
*
* Function: int puts(const char *s)
* Description: Writes the character s, to the standard output
*
*/
int puts(const unsigned char *s);

int fputi(int i, fDescriptor stream);

int puti(int i);

int fputx(int i, fDescriptor stream);

int putx(int i);

/*
*	Function:	getc
*	Description: 	Returns a char or -1 from the file given
*	Return:	the character read as an unsigned char cast to an int or EOF on end of file or error
*/
int getc(int fd);

/*
*	Function:	getchar
*	Description: 	Returns a char or -1 from stdin
*	Return:	the character read as an unsigned char cast to an int or EOF on end of file or error
*/
int getchar();

/*
*	Function: ssize_t getline(unsigned char *lineptr, size_t n, int fd)
*	Description: reads  an  entire  line  from  stream on lineptr, returns
*	the amount of characters read (<= n) or EOF if EOF was reached. The
*	line is null-terminated and includes the newline character, if one was found.
*	Parameters:
*		lineptr: the destination buffer where to store the line
*		n: the max amount of characters to read
*		fd: the file where to read the line	
*	Return: the number of characters read or -1 if EOF was reached or an 
*	error occured.	
*
*/
ssize_t getline(unsigned char *lineptr, size_t n, int fd);

#endif
