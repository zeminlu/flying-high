/*
*	stdio.c
*
*	Standard Input/Output library functions
*	Author: Santiago Perez De Rosso
*
*/

/*
* Includes
*
*/
#include "stdio.h"
#include "string.h"
#include "controllers.h"
#include "convertions.h"

/*
* Defines
*/
#define NIBBLES_PER_INT sizeof(int) * 2

typedef struct{
	int fd;
} file_tCDT;

/*
* Public functions
* ===============
*/
unsigned char charBuf[1];
int
putc(int c, fDescriptor stream){
	charBuf[0]=(unsigned char) c;
	return writeController(stream,(const void *) &c, 1);
}

int
fputs(const unsigned char *s, fDescriptor stream){
	int count;
	if(s != NULL){
		count=strlen((char *)s);
		return writeController(stream, (const void *)s, count);
	}
	return writeController(stream, (const void *) "null", 4);
}

int
putchar(int c){
	return putc(c, stdout);
}

int
puts(const unsigned char *s){
	return fputs(s, stdout);
}

int fputi(int i, fDescriptor stream) {
	char msg[10];

	memset(msg, '\0', 10);
	intToString(i, msg);	

	return fputs((const unsigned char *)msg, stream);
}

int puti(int i) {
	return fputi(i, stdout);
}

int fputx(int num, fDescriptor stream) {
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
	return fputs((const unsigned char *)msg, stream);
}

int putx(int num) {
	return fputx(num, stdout);
}

int
getc(int fd){
	unsigned char a;
//	while(!readController(fd, &a, 1));
//	_debug();
	readController(fd, &a, 1);
//	_debug();
	return a;
}


int
getchar(){
	return getc(stdin);
}

ssize_t
getline(unsigned char *lineptr, size_t n, int fd){
	int i;
	unsigned char c;
	for(i=0; i<n-2 && (c=getc(fd)) != EOF && c!='\n'; ++i)
		lineptr[i]=c;
	if(c=='\n'){
		lineptr[i]=c;
		++i;
	}
	lineptr[i]='\0';
	return i;
}


