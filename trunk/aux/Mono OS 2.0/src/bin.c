/*
 *	bin.c
 *
 */
#include "bin.h"
#include "stdio.h"
#include "shell.h"
#include "sysprocess.h"
#include "kasm.h"

void idle(void *args) {
	while(1)
		asm volatile ("hlt");
	return;
}

void fun1(int args) {	
    while(1){
	puts("1 \n");
	puts("Press a key to continue: \n");
	getchar();
	}
}

void fun2(int args) {
  //  *(&args + 16) = 20;
    
    int i=2000;
	while(1){
		puti(i--);
		puts("\n");
	}
}

void topProcess(int args) {
	int i;
	int status = FALSE;
	PROCESS *processTable = getProcessTable();

			setVideoOffSet(1, 0);
			clearScreen(1);
	while(1){
		if(getFocusTTY() != sysGetTty(sysGetpid()))
			status = TRUE;
	    //clearScreen(0);
	    if(getTty(sysGetpid()) == getFocusTTY()){
			if(status){
				setVideoOffSet(1, 0);
				clearScreen(1);
			}
			setVideoOffSet(1, 0);
			status = FALSE;
		}
	puts((unsigned char *)"TOP - Process List                                                          \n");
	puts((unsigned char *)"pid    name            state        priority\n");
	for ( i = 0; i < MAX_PROCESS; ++i ) {
		if ( processTable[i].pid == -1 )
			continue;

		puti(i);
		puts((unsigned char *)"     ");
		puts((unsigned char *)processTable[i].name);
		puts((unsigned char *)"       ");
		switch ( processTable[i].state ) {
			case READY:
				puts((unsigned char *)"READY");
				break;
			case BLOCKED:
				puts((unsigned char *)"BLOCKED");
				break;
			case WAITING_PID:
				puts((unsigned char *)"WAITING PID");
				break;
			case WAITING_TIME:
				puts((unsigned char *)"WAITING TIME");
				break;
			case WAITING_CHILD:
				puts((unsigned char *)"WAITING_CHILD");
				break;
			case TERMINATED:
				puts((unsigned char *)"TERMINATED");
				break;
			case RUNNING:
				puts((unsigned char *)"RUNNING");
				break;
			case DEAD:
				puts((unsigned char *)"DEAD");
				break;
			default:
				puts((unsigned char *)"-------");
				break;
		}
		puts((unsigned char *)"     ");
		puti(processTable[i].priority);
		puts((unsigned char *)"     \n");
	}
	}
}

void init(void *args) {
	int i;
	pid_t pid;
/*
	setTty(sysGetpid(), 0);
	if ( (pid = createProcess("idle", idle, 0, BACKGROUND)) == -1 )  {
			puts((const unsigned char *)"ERROR: A Shell could not be created.\n");
			puts((const unsigned char *)getProcessErrorMsg());
			_Cli();
			while(1);
	}*//*
	if ( (pid = createProcess("sh", shellStart, 0, FOREGROUND)) == -1 )  {
		puts((const unsigned char *)"ERROR: A Shell could not be created.\n");
		puts((const unsigned char *)getProcessErrorMsg());
		_Cli();
		while(1);
	}
	setTty(pid, 0);
	if ( (pid = createProcess("sh", shellStart, 0, FOREGROUND)) == -1 )  {
		puts((const unsigned char *)"ERROR: A Shell could not be created.\n");
		puts((const unsigned char *)getProcessErrorMsg());
		_Cli();
		while(1);
	}
	setTty(pid, 1);
*/



/*	if ( (pid = createProcess("sh", fun1, 0, FOREGROUND)) == -1 )  {
		puts((const unsigned char *)"ERROR: A Shell could not be created.\n");
		puts((const unsigned char *)getProcessErrorMsg());
		_Cli();
		while(1);
	}
	setTty(pid, 0);
	if ( (pid = createProcess("sh", fun2, 0, FOREGROUND)) == -1 )  {
		puts((const unsigned char *)"ERROR: A Shell could not be created.\n");
		puts((const unsigned char *)getProcessErrorMsg());
		_Cli();
		while(1);
	}
	setTty(pid, 1);
*/
	for ( i = 0; i < MAXAMOUNTOFTTYS; ++i ) {
		if ( (pid = createProcess("sh", shellStart, NULL, FOREGROUND)) == -1 ) {
			puts((const unsigned char *)"ERROR: A Shell could not be created.\n");
			puts((const unsigned char *)getProcessErrorMsg());
		}
		setTty(pid, i);
	}
	
	while(1)
		asm volatile ("hlt");
	
	return;
}	
