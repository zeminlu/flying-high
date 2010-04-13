/* 
 *  File: writeprintf.c
 *  Author: Lucas Pizzagalli
 *  Version: 02/06/2009 - 20:17
 *  Description: 
 *  	contains function to helt printf to write to video correctly.
 *  	Makeing enter when a "\n" is present in string. It is thought
 *  	to be used by printf.
 */       

#include "videoDriver.h"
#include "string.h"
#include "controllers.h"

/*
 * writeprintf:
 *		print the string a to screen  
 *		
 *
 */
	
int 
writeprintf( unsigned char * a)
{
	writeController(stdout, a, strlen((char *)a) -1);
	return 0;
}

