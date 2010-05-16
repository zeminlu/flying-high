/**
 *	\file int80.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _INT80_H_
#define _INT80_H_

/*
 * Function: void * int80( int eax, void * ebx, void * ecx, void * edx)
 *	
 *		Description: copy to the processor registers the parameters
 *
 */
/**
 * \fn void * int80( int eax, void * ebx, void * ecx, void * edx )
 *
 * 		\brief Brief.
 *
 * 		\param eax ParamBrief.
 * 		\param ebx ParamBrief.
 * 		\param ecx ParamBrief.
 * 		\param edx ParamBrief.
 * 		
 * 		\return Description.

 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void * int80( int eax, void * ebx, void * ecx, void * edx );

/**
 * \fn void * int80ext(int eax, void *ebx, void *ecx, void *edx, void *edi, void *esi)
 *
 * 		\brief Brief.
 *
 * 		\param eax ParamBrief.
 * 		\param ebx ParamBrief.
 * 		\param ecx ParamBrief.
 * 		\param edx ParamBrief.
 * 		\param edi ParamBrief.
 * 		\param esi ParamBrief.
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
void * int80ext(int eax, void *ebx, void *ecx, void *edx, void *edi, void *esi); 

#endif