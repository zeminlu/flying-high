#include "bin.h"


void init(void *args){
	int i;/*, status;*/
	pid_t pid;
	
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
	
	/*for (i = 0 ; i < MAX_TTY ; ++i){
		waitpid(i + 1, &status);
	}*/
	/*
	kill(pid);
	rebootSystem();
	*/	
	while(1){
		asm volatile ("hlt");
	}
	return;
}

void top(char * args) {
	int i, j, c1 = 6, c2 = 7, c3 = 13, c4 = 8, c5 = 9, c6 = 15, c7 = 7, c8 = 15;
	int aux;/*status = FALSE, vCol, vRow, readPointer, writePointer, readCol, readRow, writeCol, writeRow;*/
	process_t *processTable;
	char *title = "TOP - Process List", *l1 = "PID", *l2 = "PPID", *l3 = "Name", *l4 = "% CPU", *l5 = "Childs", *l6 = "Used Mem (B)", *l7 = "Prio", *l8 = "State", *tmp, tmp2[10];

	processTable = getProcessTable();

	/*	setVideoOffSet(1, 0);*/
	/*while(1){*/
		setProcessAtomicity(getpid(), ATOMIC);
		refreshProcessesCPUs();
		clearProcessesTicks();
		setProcessAtomicity(getpid(), UNATOMIC);	
	/*	if(getFocusedTTY() != getTty(getpid()))
			status = TRUE;
	    if(getTty(getpid()) == getFocusedTTY()){
			if(status){
				//setVideoOffSet(1, 0);
				clearScreen();
			}
		//	setVideoOffSet(1, 0);
			status = FALSE;
		}*/
		for (j = 0 ; j < (SCREEN_WIDTH - strlen(title)) / 2 ; ++j)
			putchar(' ');
		puts(title);
		putchar('\n');
		for (j = 0 ; j < SCREEN_WIDTH ; ++j){
			putchar('-');
		}
		putchar(' ');
		puts(l1);
		for (j = 0 ; j < c1 - (strlen(l1)) - 2 ; ++j)
			putchar(' ');
		putchar('|');
		putchar(' ');
		puts(l2);
		for (j = 0 ; j < c2 - (strlen(l2)) - 2 ; ++j)
			putchar(' ');
		putchar('|');
		putchar(' ');
		for (j = 0 ; j < (c3 - (strlen(l3)) - 2) / 2 ; ++j)
			putchar(' ');
		puts(l3);
		for (j = 0 ; j < (c3 - (strlen(l3)) - 2) / 2 ; ++j)
			putchar(' ');
		putchar(' ');	
		putchar('|');
		putchar(' ');
		puts(l4);
		for (j = 0 ; j < c4 - (strlen(l4)) - 2 ; ++j)
			putchar(' ');
		putchar('|');
		putchar(' ');
		puts(l5);
		for (j = 0 ; j < c5 - (strlen(l5)) - 2 ; ++j)
			putchar(' ');
		putchar('|');
		putchar(' ');
		puts(l6);
		for (j = 0 ; j < c6 - (strlen(l6)) - 2 ; ++j)
			putchar(' ');
		putchar('|');
		putchar(' ');
		puts(l7);
		for (j = 0 ; j < c7 - (strlen(l7)) - 2 ; ++j)
			putchar(' ');
		putchar('|');
		putchar(' ');
		puts(l8);
		for (j = 0 ; j < c8 - (strlen(l8)) - 1 ; ++j)
			putchar(' ');
		for (j = 0 ; j < SCREEN_WIDTH ; ++j){
			putchar('-');
		}
		for ( i = 0; i < MAX_PROCESS; ++i ) {
			if ( processTable[i].pid == -1 )
				continue;
			putchar(' ');	
			puti(aux = processTable[i].pid);
			for (j = 0 ; j < c1 - ((aux > 9) ? 2 : 1) - 2 ; ++j)
				putchar(' ');
			putchar('|');
			putchar(' ');
			puti(aux = processTable[i].ppid);
			for (j = 0 ; j < c2 - ((aux > 9) ? 2 : 1) - 2 ; ++j)
				putchar(' ');
			putchar('|');
			putchar(' ');
			puts(tmp = processTable[i].name);
			for (j = 0 ; j < c3 - (strlen(tmp)) - 2 ; ++j)
				putchar(' ');
			putchar('|');
			putchar(' ');
			doubleToString(processTable[i].cpuPercent, tmp2);
			puts(tmp2);
			for (j = 0 ; j < c4 - strlen(tmp2) - 2 ; ++j)
				putchar(' ');
			putchar('|');
			putchar(' ');
			puti(aux = processTable[i].childsQty);
			for (j = 0 ; j < c5 - ((aux > 9) ? 2 : 1) - 2 ; ++j)
				putchar(' ');	
			putchar('|');
			putchar(' ');
			puti(aux = (processTable[i].dataUmalloc.mallocMem.allocp - (char *)processTable[i].dataUmalloc.mallocMem.address));
			for (j = 0 ; j < c6 - ((aux / 10 > 9) ? 3 : ((aux / 10) ? 2 : 1)) - 2 ; ++j)
				putchar(' ');
			putchar('|');
			putchar(' ');
			puti(processTable[i].priority);
			for (j = 0 ; j < c7 - 1 - 2 ; ++j)
				putchar(' ');		
			putchar('|');
			putchar(' ');
			switch ( processTable[i].state ) {
				case READY:
					puts(tmp = "READY");
					break;
				case BLOCKED:
					puts(tmp = "BLOCKED");
					break;
				case WAITING_PID:
					puts(tmp = "WAITING PID");
					break;
				case WAITING_TIME:
					puts(tmp = "WAITING TIME");
					break;
				case WAITING_CHILD:
					puts(tmp = "WAITING_CHILD");
					break;
				case TERMINATED:
					puts(tmp = "TERMINATED");
					break;
				case RUNNING:
					puts(tmp = "RUNNING");
					break;
				case DEAD:
					puts(tmp = "DEAD");
					break;
				default:
					puts(tmp = "-------");
					break;
			}
			for (j = 0 ; j < c8 - strlen(tmp) - 1 ; ++j)
				putchar(' ');
		}
		for (j = 0 ; j < SCREEN_WIDTH ; ++j){
			putchar('-');
		}
/*	}*/
}
