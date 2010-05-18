/**
 *	\file kMalloc.c
 *
 *		\brief The kernel malloc module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "../inc/kMalloc.h"

/*Falta agregar el true y el false*/

static int asigment = FALSE;
static memArea_t auxMem;

static memArea_t * getMem(){	
	if(!asigment){
		asigment = TRUE;
		auxMem.allocp = NULL;
		auxMem.address = memmap(TRUE);
		auxMem.size = (PAGES_PER_FRAME * PAGE_SIZE);
	}

	return &auxMem;
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