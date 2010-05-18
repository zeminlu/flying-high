/**
 *	\file bin.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "bin.h"

extern process_t *processTable;

void init(void *args){
	int i, status;
	pid_t pid;
	process_t *processTable;
	
	processTable = getProcessTable();
	
	setProcessAtomicity(getpid(), ATOMIC);
	if ( (pid = createProcess("welcome", (void (*)(void *))welcome, NULL, FOREGROUND)) == -1 ) {
		puts("ERROR: Welcome could not be created.\n");
	}		
	setTty(pid, 0);
	setProcessAtomicity(getpid(), UNATOMIC);
	
	while(processTable[0].childsQty){
		asm volatile("hlt");
	}
	
	setProcessAtomicity(getpid(), ATOMIC);
	for ( i = 0; i < MAX_TTY; ++i ) {
		if ( (pid = createProcess("sh", (void (*)(void *))shell, NULL, FOREGROUND)) == -1 ) {
			puts("ERROR: A Shell could not be created.\n");
		}		
		setTty(pid, i);
		setTTYMode(pid, TTY_CANONICAL);
	}	
	setProcessAtomicity(getpid(), UNATOMIC);
		
	while(processTable[0].childsQty){
		wait(&status);
		asm volatile("hlt");
	}
	
	setProcessAtomicity(getpid(), ATOMIC);
	if ( (pid = createProcess("goodbye", (void (*)(void *))goodbye, NULL, FOREGROUND)) == -1 ) {
		puts("ERROR: Goodbye could not be created.\n");
	}		
	setTty(pid, 0);
	setProcessAtomicity(getpid(), UNATOMIC);
	
	while(processTable[0].childsQty){
		asm volatile("hlt");
	}
	
	rebootSystem();
	
	return;
}

void top(char * args) {
	int i, j, c1 = 6, c2 = 7, c3 = 13, c4 = 8, c5 = 9, c6 = 15, c7 = 7, c8 = 15;
	int aux, dig = 1;
	process_t *processTable;
	char *title = "TOP - Process List", *l1 = "PID", *l2 = "PPID", *l3 = "Name", *l4 = "% CPU", *l5 = "Childs", *l6 = "Used Mem (B)", *l7 = "Prio", *l8 = "State", *tmp, tmp2[10];

	processTable = getProcessTable();
	clearTTYScreen();
	
	putchar('\n');
	
	for (j = 0 ; j < (SCREEN_WIDTH - strlen(title)) / 2 ; ++j)
		putchar(' ');
	puts(title);
	putchar('\n');
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
	while(1){
		asm volatile("hlt");
		setProcessAtomicity(getpid(), ATOMIC);
		refreshProcessesCPUs();
		clearProcessesTicks();
		setTTYCursorPosition(6, 0);
		setProcessAtomicity(getpid(), UNATOMIC);	
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
			while (aux / 10 > 0 ){
				aux /= 10;
				++dig;
			}
			for (j = 0 ; j < c6 - dig - 2 ; ++j)
				putchar(' ');
			dig = 1;
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
	}
}

void welcome(){
	unsigned curTime;
	int j = 0;
	char *msg;
	
	curTime = upTime();
	clearTTYScreen();
	
	msg = "\n\tWelcome, you're Flying-High at 0.8 meters!!\n\n";
	
	puts(msg);

	puts("\t\tInitializing Drivers..............................................");

	++j;
	
	while(upTime() < curTime + j * TICKS_PER_SECOND);

	puts("Done.\n");
	
	puts("\n\t\tEnabling Interrupts...............................................");

	++j;
	
	while(upTime() < curTime + j * TICKS_PER_SECOND);
	
	puts("Done.\n");	

	puts("\n\tInitializing Shells...................................................");

	initGlobalDataShell();
	
	++j;
	
	while(upTime() < curTime + j * TICKS_PER_SECOND);
	
	puts("Done.\n");
	
	puts("\n\tReady.\n\n");
	
	return;
}

void goodbye(){
	unsigned curTime;
	int j = 0;
	
	clearTTYScreen();
	
	curTime = upTime();
	
	puts("\nNo processes running, starting system reboot...\n\n");
	
	++j;
	
	while(upTime() < curTime + j * TICKS_PER_SECOND);
	
	puts("\tShutting down drivers...");
	
	++j;
	
	while(upTime() < curTime + j * TICKS_PER_SECOND);
	
	puts("...Done.\n\n");
	
	puts("Rebooting now...\n");
	
	++j;
	
	while(upTime() < curTime + j * TICKS_PER_SECOND);
	
	return;
}

void printA(){
	
	while (1){
		putchar('A');
		asm volatile("hlt");
	}
	return;
}

void printB(){
	while (1){
		putchar('B');
		asm volatile("hlt");
	}
	return;
}

void nothing(){
		
	while (1){
		getchar();
		asm volatile("hlt");
	}
	return;
}

void pageFault(){
	char * aux;
	
	puts("Quiero escribir en  la posicion de memoria ");
	if(getpid() - 1 >= 0){
		aux = processTable[getpid() - 1].dataUmalloc.mallocMem.allocp;
		putx((int)aux);
		*aux = 'a';	
	}else{
		aux = processTable[getpid() + 1].dataUmalloc.mallocMem.allocp;
		putx((int)aux);
		*aux = 'a';
	}
	return;
}

