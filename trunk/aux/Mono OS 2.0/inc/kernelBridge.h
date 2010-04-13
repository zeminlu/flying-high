/*
 *	kernelBridge.h
 *
 * 		Makes the change between the kernel space and the user space.
 *
 * 		Author: Guido Marucci Blas
 *
 */

#ifndef KERNELBRIDGE_H_
#define KERNELBRIDGE_H_

void enterKernelSpace(void);

void exitKernelSpace(int);

#endif
