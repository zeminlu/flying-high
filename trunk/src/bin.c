#include "bin.h"


void init(void *args){
	int i;
	pid_t pid;
	
/*	pid = createProcess("chupala", chupala, NULL, FOREGROUND);
	printChar(pid + 0x30);
	setTty(pid, 0);
	refreshScreen();	
	initDataShell();
	for ( i = 1; i < 6; ++i ) {

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
*/	

	initGlobalDataShell();
	
	for ( i = 0; i < MAX_TTY; ++i ) {
		setProcessAtomicity(getpid(), ATOMIC);
		if ( (pid = createProcess("sh", (void (*)(void *))shell, NULL, FOREGROUND)) == -1 ) {
			puts("ERROR: A Shell could not be created.\n");
		}		
		setTty(pid, i);
		sysSetTTYFocusedProcess(pid, i);
		setProcessAtomicity(getpid(), UNATOMIC);
	}
		
	pid = createProcess("idle", idle, NULL, BACKGROUND);
	setTty(pid, 0);
		
	while(1){
		asm volatile ("hlt");
	}
	return;
}

void top(char * args) {
	int i;
	int status = FALSE;
	process_t *processTable = getProcessTable();

/*	setVideoOffSet(1, 0);*/
/*	clearScreen();
	while(1){
	*/
		if(getFocusedTTY() != getTty(getpid()))
			status = TRUE;
	    if(getTty(getpid()) == getFocusedTTY()){
			if(status){
				/*setVideoOffSet(1, 0);*/
/*				clearScreen();*/
			}
			/*setVideoOffSet(1, 0);*/
			status = FALSE;
		}
		puts("TOP - Process List                                                          \n");
		puts("pid    name            state        priority\n");
		for ( i = 0; i < MAX_PROCESS; ++i ) {
			if ( processTable[i].pid == -1 )
				continue;

			puti(i);
			puts("     ");
			puts(processTable[i].name);
			puts("       ");
			switch ( processTable[i].state ) {
				case READY:
					puts("READY");
					break;
				case BLOCKED:
					puts("BLOCKED");
					break;
				case WAITING_PID:
					puts("WAITING PID");
					break;
				case WAITING_TIME:
					puts("WAITING TIME");
					break;
				case WAITING_CHILD:
					puts("WAITING_CHILD");
					break;
				case TERMINATED:
					puts("TERMINATED");
					break;
				case RUNNING:
					puts("RUNNING");
					break;
				case DEAD:
					puts("DEAD");
					break;
				default:
					puts("-------");
					break;
			}
			puts("     ");
			puti(processTable[i].priority);
			puts("     \n");
		}
/*	}*/
}