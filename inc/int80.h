/**
 *	\file int80.h
 *
 *		\brief int 80 generic calls.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _INT80_H_
#define _INT80_H_

/**
 * \fn void * int80( int eax, void * ebx, void * ecx, void * edx )
 *
 * 		\brief copy the parameters to the processor registers.
 *
 * 		\param eax eax register.
 * 		\param ebx ebx register.
 * 		\param ecx ecx register.
 * 		\param edx edx register.
 * 		
 * 		\return The value returned by the function that the int 80 invoked.
 *
 */
void * int80( int eax, void * ebx, void * ecx, void * edx );

/**
 * \fn void * int80ext(int eax, void *ebx, void *ecx, void *edx, void *edi, void *esi)
 *
 * 		\brief copy the parameters to the processor registers.
 *
 * 		\param eax eax register.
 * 		\param ebx ebx register.
 * 		\param ecx ecx register.
 * 		\param edx edx register.
 * 		\param edi edi register.
 * 		\param esi esi register.
 * 		
 * 		\return The value returned by the function that the int 80 invoked.
 *
 */
void * int80ext(int eax, void *ebx, void *ecx, void *edx, void *edi, void *esi); 

#endif