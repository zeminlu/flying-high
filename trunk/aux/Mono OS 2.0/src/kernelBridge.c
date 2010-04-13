/*
 *	kernelBridge.h
 *
 * 		Makes the change between the kernel space and the user space.
 *
 * 		Author: Guido Marucci Blas
 *
 */
#include "kernelBridge.h"
#include "memmanager.h"
#include "sysprocess.h"

static int atomicCounter = 0;

void enterKernelSpace(void) {

	if ( atomicCounter++ == 0 ) 
		setKernelHeapPresence(TRUE);
	
	return;
}

void exitKernelSpace(int callMtasker) {
	if ( --atomicCounter == 0 ) {
		setKernelHeapPresence(FALSE);
		//if ( callMtasker )
			//multitasker2();
	}
	return;
}
