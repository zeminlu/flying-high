#include "../inc/kMalloc.h"

/*Falta agregar el true y el false*/

static int asigment = FALSE;

static memArea_t getMem(){
	static memArea_t auxMem;
	
	puts("Entro a getMem\n");
	
	if(!asigment){
		asigment = TRUE;
		auxMem.address = memmap(TRUE);
		auxMem.size = (8 * PAGE_SIZE);
	}
	puts("Salio del getMem() con address = ");
	putx((int)auxMem.address);
	puts("\n");
	flushStdScreen();
	return auxMem;
}

void kFree(void *ap){
	sysFree(ap,getMem);
	return;
}
void *kMalloc(size_t nbytes){
	puts("Entro\n");
	
	return sysMalloc(nbytes,getMem);
}
void *kRealloc(void *ap, size_t size){
	return sysRealloc(ap, size,getMem);
}