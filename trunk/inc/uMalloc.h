#ifndef _U_MALLOC_H_
#define	_U_MALLOC_H_

#include "memModule.h"
#include "sysMalloc.h"
#include "process.h"

void free(void *ap);

void *malloc(size_t nbytes);

void *realloc(void *ap, size_t size);

#endif