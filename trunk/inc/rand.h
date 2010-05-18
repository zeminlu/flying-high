/**
 *	\file rand.h
 *
 *		\brief Random number generation module.
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
 * 		\brief This function loads the amount of ticks. It uses
 *				a system call.
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
 * 		\brief Generates a random number with a linear equation.
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
 * 		\brief Sets the seed to generate random numbers
 *
 * 		\param seedP The seed to be set.
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