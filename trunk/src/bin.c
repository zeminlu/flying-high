#include "bin.h"

extern dataSlotShell *tableDataShell;
void init(void *args){
	int i;
	pid_t pid;
	/*
	pid = createProcess("puto", puto, NULL, FOREGROUND);
	setTty(pid, 3);
	printChar(pid + 0x30);
	
	pid = createProcess("chupala", chupala, NULL, FOREGROUND);
	setTty(pid, 2);
	printChar(pid + 0x30);
	
	pid = createProcess("puto", puto2, NULL, FOREGROUND);
	setTty(pid, 1);
	printChar(pid + 0x30);
	
	pid = createProcess("chupala", chupala2, NULL, FOREGROUND);
	setTty(pid, 4);
	printChar(pid + 0x30);
	*/
	initDataShell();
	for ( i = 0; i < 2; ++i ) {
		if ( (pid = createProcess("sh", shell, NULL, FOREGROUND)) == -1 ) {
			puts("ERROR: A Shell could not be created.\n");
		}
		setTty(pid, i);
		printChar(pid + 0x30);
	}
	
	while(1){
		asm volatile ("hlt");
	}
	return;
}