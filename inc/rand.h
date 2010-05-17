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
 * 		\brief This function load the amount of ticks. It calls
 *				to the a system call.
 *
 * 		\return The amount of tick
 *
 * 		Use:
 * 		\code
 *			srand(timeRand());
 *		\endcode
 *
 * 		\sa _system_uptime
 *
 */
unsigned int timeRand();

/**
 * \fn int rand()
 *
 * 		\brief Generate a random number with a linear equation.
 *
 * 		\return A random number
 *
 * 		Use:
 * 		\code
 *			int ran = rand();
 *		\endcode
 *
 */
int rand();

/**
 * \fn void srand( unsigned int seedP )
 *
 * 		\brief Set the seed to generate random numbers
 *
 * 		\param seedP The seed to set.
 * 		
 * 		Use:
 * 		\code
 *			srand(timeRand());
 *		\endcode
 *
 * 		\sa
 *
 */
void srand( unsigned int seedP );

#endif