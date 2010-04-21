#include "../inc/kMalloc.h"

/*Falta agregar el true y el false*/

static int asigment = FALSE;

static memArea_t getMem(){
	static memArea_t auxMem;
	if(!asigment){
		asigment = TRUE;
		auxMem.address = memmap(TRUE);
		auxMem.size = (8 * PAGE_SIZE);
	}
	return auxMem;
}

void kFree(void *ap){
	sysFree(ap,getMem);
	return;
}
void *kMalloc(size_t nbytes){
	return sysMalloc(nbytes,getMem);
}
void *kRealloc(void *ap, size_t size){
	return sysRealloc(ap, size,getMem);
}