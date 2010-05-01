#include "../inc/kernelDepth.h"

static int atomicCounter = 0;

void increaseKernelDepth(void) {

	if ( atomicCounter++ == 0 ){ 
		setKernelHeapPresence(TRUE);
	}
	
	return;
}

void decreaseKernelDepth(void) {
	if ( --atomicCounter == 0 ){
		setKernelHeapPresence(FALSE);
	}
	return;
}
