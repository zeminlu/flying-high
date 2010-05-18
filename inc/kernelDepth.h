/**
 *	\file kernelDepth.h
 *
 *		\brief Kernel syscalls depth utilities.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _KERNELDEPTH_H_
#define _KERNELDEPTH_H_

#include "memModule.h"
#include "kasm.h"

/**
 * \fn void increaseKernelDepth(void)
 *
 * 		\brief This function increases the depth relating to the iteration of system calls
 *				maintaining the presence of the kernel heap pages.
 *
 * 		\sa decreaseKernelDepth()
 *
 */
void increaseKernelDepth(void);

/**
 * \fn void decreaseKernelDepth(void)
 *
 * 		\brief This function decreases the depth relating to the iteration of system calls
 *				maintaining the presence of the kernel heap pages if needed.
 *
 * 		\sa decreaseKernelDepth()
 *
 */
void decreaseKernelDepth(void);

#endif