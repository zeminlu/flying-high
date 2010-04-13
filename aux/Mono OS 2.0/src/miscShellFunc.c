#include "stdio.h"
#include "stdlib.h"
#include "cmdDispatch.h"
#include "process.h"

void
killProc(int paramFd){
    char opt[MAX_OPT_SIZE]; 
    read(paramFd, opt, sizeof(opt));   
   	if( atoi(opt) <= 0 ){
	    puts((unsigned char *) "You can't kill that one! \n");
	    return;
	}
    kill(atoi(opt), SIGKILL);
}

void
changeProcPrio(int paramFd){
    char opt[MAX_OPT_SIZE], *procPid, *newPrio; 
    read(paramFd, opt, sizeof(opt));   
    pid_t pid;
    int newIntPrio;
    procPid=strtok(opt, " ");
    newPrio=procPid + strlen(procPid) + 1;
    puts((unsigned char *) " Changing process of pid: ");
    puti(pid=atoi(procPid));
    puts((unsigned char *) " for ");
    puti(newIntPrio=atoi(newPrio));
    if ( pid <= 0 ){ 
	   puts("Error: Invalid pid");
	return;
    }
    if ( newIntPrio <= 0 ){ 
	   puts("Error: Invalid priority");
	return;
    }
    puts((unsigned char *) "\n About to set \n"); 
    if ( setpriority(pid, newIntPrio) == -1 )
	   puts("Error: Priority could not be set.\n");
}

