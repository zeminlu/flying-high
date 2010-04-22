#include "../inc/sysMalloc.h"
/*
 * 	Defines
 * 	=======
 */
static char * allocp = NULL;

static void *baseMalloc(size_t nbytes, memArea_t * memArea){
	unsigned int aux1;
	unsigned int aux2;
	
	aux1 = (unsigned int)memArea->address;
	aux2 = (unsigned int)memArea->size;
	if(allocp == NULL){
		allocp = memArea->address;
	}
	
	if((aux1 + aux2 - (unsigned int)allocp) >= nbytes){
		allocp += nbytes;
		return allocp - nbytes;
	}else{
		return NULL;
	}

}
/*
 * 	Public Functions
 * 	================
 */
void sysFree(void *ap, memArea_t * (*getMemoryArea)()){
	;
}

void *sysRealloc(void *ptr, size_t size, memArea_t * (*getMemoryArea)()){
	void *ret;

	if ( ptr == NULL ) {
		return sysMalloc(size, getMemoryArea);
	}
	if( (ret=sysMalloc(size, getMemoryArea)) != NULL ){
	    memcpy(ret, ptr, size);

	}
	return ret;
}

void *sysMalloc(size_t nbytes, memArea_t * (*getMemoryArea)()){
	void * debug;
	memArea_t * memArea;
	
	memArea = getMemoryArea();
	
	debug  = baseMalloc(nbytes, memArea);
	/*
	puts("Valor en la estructura del address = ");
	putx((int)memArea->address);
	puts("\n");
	flushStdScreen();
	*/
	return debug;
}
