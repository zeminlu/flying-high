#include "bin.h"

extern dataSlotShell *tableDataShell;
void init(void *args){
/*	int i;*/
	pid_t pid;
		
	pid = createProcess("chupala", chupala, NULL, FOREGROUND);
	printChar(pid + 0x30);
	setTty(pid, 0);
	refreshScreen();
		
	pid = createProcess("puto", puto, NULL, FOREGROUND);
	setTty(pid, 1);
	printChar(pid + 0x30);
	refreshScreen();
	
	pid = createProcess("de", de, NULL, FOREGROUND);
	setTty(pid, 2);
	printChar(pid + 0x30);
	refreshScreen();
	
	pid = createProcess("mierda", mierda, NULL, FOREGROUND);
	setTty(pid, 3);
	printChar(pid + 0x30);
	refreshScreen();

	pid = createProcess("anda", anda, NULL, FOREGROUND);
	printChar(pid + 0x30);
	setTty(pid, 4);
	refreshScreen();

	pid = createProcess("todo", todo, NULL, FOREGROUND);
	printChar(pid + 0x30);
	setTty(pid, 5);
	refreshScreen();

	pid = createProcess("carajo", carajo, NULL, FOREGROUND);
	printChar(pid + 0x30);
	setTty(pid, 6);
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
		
	while(1){
		asm volatile ("hlt");
	}
	return;
}