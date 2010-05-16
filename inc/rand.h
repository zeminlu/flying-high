/**
 *	\file rand.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#ifndef RAND
#define RAND

#include "syscall.h"

#define RAND_MAX 42949679

/**
 * \fn unsigned int timeRand()
 *
 * 		\brief Brief.
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
unsigned int timeRand();

/**
 * \fn int rand()
 *
 * 		\brief Brief.
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
int rand();

/**
 * \fn void srand( unsigned int seedP )
 *
 * 		\brief Brief.
 *
 * 		\param seedP ParamBrief.
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
void srand( unsigned int seedP );

#endif