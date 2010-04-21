#include "../inc/uMalloc.h"

static int asigment = FALSE;

static memArea_t getMem(){
	static memArea_t auxMem;
	if(!asigment){
		asigment = TRUE;
		auxMem.address = memmap(FALSE);
		auxMem.size = (6 * PAGE_SIZE);
	}
	return auxMem;

}

void free(void *ap){
	sysFree(ap,getMem);
	return ;
}
void *malloc(size_t nbytes){
	return sysMalloc(nbytes,getMem);
}
void *realloc(void *ap, size_t size){
	return sysRealloc(ap, size,getMem);
}
