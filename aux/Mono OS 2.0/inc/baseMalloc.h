#include "stdio.h"


typedef struct {
    void *address;
    size_t size;
} memArea_t;

void sysFree(void *ap, memArea_t (*getMemoryArea)());
void *sysMalloc(size_t nbytes, memArea_t (*getMemoryArea)());
void *sysRealloc(void *ap, size_t size, memArea_t (*getMemoryArea)());
