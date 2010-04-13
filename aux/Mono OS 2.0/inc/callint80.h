/**
 *	File: callint80.h
 *		Definitions of calling80.asm
 */

/*
 * Function: void * callInt80( int eax, void * ebx, void * ecx, void * edx)
 *	
 *		Description: copy to the processor registers the parameters
 *
 */
 void * callInt80( int eax, void * ebx, void * ecx, void * edx );


 void * callInt802( int eax, void * ebx, void * ecx, void * edx, void *edi, void *esi);
