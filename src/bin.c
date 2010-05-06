#include "bin.h"

void init(void *args){
	int i;
	pid_t pid;
	pid = createProcess("puto", puto, NULL, FOREGROUND);
	setTty(pid, 3);


	pid = createProcess("chupala", chupala, NULL, FOREGROUND);
	setTty(pid, 2);

	
	for ( i = 1; i < 2; ++i ) {
		if ( (pid = createProcess("sh", shell, NULL, FOREGROUND)) == -1 ) {
			puts("ERROR: A Shell could not be created.\n");
		}
		setTty(pid, i);
	}
	
	
	
	while(1){
		asm volatile ("hlt");
	}
	return;
}