/**
 *	\file uMalloc.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "../inc/uMalloc.h"

static memArea_t * getMem(){
	return &(runningProcess->dataUmalloc.mallocMem);
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
