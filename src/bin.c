#include "bin.h"

extern dataSlotShell *tableDataShell;
void init(void *args){
	int i;
	pid_t pid;

	//_Cli();
		
	pid = createProcess("puto", puto, NULL, FOREGROUND);
	printChar(pid + 0x30);
	setTty(pid, 0);
	refreshScreen();
	
	
	pid = createProcess("chupala", chupala, NULL, FOREGROUND);
	setTty(pid, 1);
	printChar(pid + 0x30);
	refreshScreen();
	
	pid = createProcess("puto", puto2, NULL, FOREGROUND);
	setTty(pid, 2);
	printChar(pid + 0x30);
	refreshScreen();
	
	_debug();
	pid = createProcess("chupala", chupala2, NULL, FOREGROUND);
	setTty(pid, 3);
	printChar(pid + 0x30);
	refreshScreen();
	
/*	initDataShell();
	for ( i = 0; i < 3; ++i ) {
		if ( (pid = createProcess("sh", shell, NULL, FOREGROUND)) == -1 ) {
			puts("ERROR: A Shell could not be created.\n");
		}
		printChar(pid + 0x30);
		setTty(pid, i);
	}
*/		
	//_Sti();	
		
	while(1){
		asm volatile ("hlt");
	}
	return;
}