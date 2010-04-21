#ifndef K_MALLOC_H_
#define	K_MALLOC_H_

#include "memModule.h"
#include "sysMalloc.h"
#include "process.h"
#include "stdio.h"

void kFree(void *ap);

void *kMalloc(size_t nbytes);

void *kRealloc(void *ap, size_t size);

#endif