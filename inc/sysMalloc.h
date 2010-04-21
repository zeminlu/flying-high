#ifndef SYSMALLOC_H_
#define	SYSMALLOC_H_

#include "memModule.h"
#include "string.h"

void sysFree(void *ap, memArea_t (*getMemoryArea)());

void *sysMalloc(size_t nbytes, memArea_t (*getMemoryArea)());

void *sysRealloc(void *ap, size_t size, memArea_t (*getMemoryArea)());

#endif