#include "baseMalloc.h"
#include "memmanager.h"

static memArea_t getMemAreaForKernel(){
    memArea_t ret;
    if( (ret.address=getKernelHeap()) != NULL ){
	   ret.size = getKernelHeapSize();
    }
    return ret;
}


void *krealloc(void *ap, size_t nbytes){
    return sysRealloc(ap, nbytes, getMemAreaForKernel);
}

void *kmalloc(size_t nbytes){
    return sysMalloc(nbytes, getMemAreaForKernel);
}

void kfree(void *ap){
    return sysFree(ap, getMemAreaForKernel);
}
