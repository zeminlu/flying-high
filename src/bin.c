#include "bin.h"

void init(void *args){
	/*int i;
	pid_t pid;

	for ( i = 0; i < MAXAMOUNTOFTTYS; ++i ) {
		if ( (pid = createProcess("sh", shellStart, NULL, FOREGROUND)) == -1 ) {
			puts("ERROR: A Shell could not be created.\n");
		}
		setTty(pid, i);
	}*/
	createProcess("chupala", chupala, NULL, FOREGROUND);
	createProcess("puto", puto, NULL, FOREGROUND);

	while(1)
		asm volatile ("hlt");

	return;
}