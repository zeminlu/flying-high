#include "bin.h"

void init(void *args){
	int i;
	pid_t pid;

	for ( i = 0; i < /*MAXAMOUNTOFTTYS*/ 1; ++i ) {
		if ( (pid = createProcess("sh", shell, NULL, FOREGROUND)) == -1 ) {
			puts("ERROR: A Shell could not be created.\n");
		}
		setTty(pid, i);
	}
/*	
	pid = createProcess("chupala", chupala, NULL, FOREGROUND);
	setTty(pid, 0);
*/	pid = createProcess("puto", puto, NULL, FOREGROUND);
	setTty(pid, 1);
	
	while(1){
		asm volatile ("hlt");
	}
	return;
}