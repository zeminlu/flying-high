/**
 *	File: int80.h
 *		Definitions of calling80.asm
 */

#ifndef _INT80_H_
#define _INT80_H_

/*
 * Function: void * int80( int eax, void * ebx, void * ecx, void * edx)
 *	
 *		Description: copy to the processor registers the parameters
 *
 */
 void * int80( int eax, void * ebx, void * ecx, void * edx );
 
#endif