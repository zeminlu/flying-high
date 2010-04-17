#include "process.h"

void * memmap(){
	return (void *)int80(_SYS_MEMMAP, NULL, NULL, NULL);
}