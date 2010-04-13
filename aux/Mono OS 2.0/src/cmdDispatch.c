/*
*	File: cmdDispatch.c
*	Author: Santiago Perez De Rosso
*	Version: 02/06/09 - 4:29 
*	Description: The command dispatcher evaluates given commands and dispatch
*	the expected functionality if possible
*
*/

/*
*	Includes
*	========
*/
#include "cmdDispatch.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "genShellFunc.h"
#include "dirShellFunc.h"
#include "fileShellFunc.h"
#include "miscShellFunc.h"
#include "process.h"
#include "sysprocess.h"
#include "filesystem.h"
#include "primitives.h"
#include "bin.h"

/*
*	typedef: cmdItem
*	Description: Defines the basic item structure the command-dispatching
*	table has. cmd refers to the expected command (no "options part") and
*	its associated function.
*/
typedef struct cmdItem{
	unsigned char *cmd;
	void (*fn)(int);
} cmdItem;

cmdItem cmdTable[]={
	{(unsigned char *) "screensaver", parseScreensaverOpt}, 
	{(unsigned char *) "help", showHelp}, 
	{(unsigned char *) "authors", showAuthors}, 
	{(unsigned char *) "clear", clearScreenWrap}, 
	{(unsigned char *) "fillscreen", fillScreen},
	{(unsigned char *) "ls", ls},
	{(unsigned char *) "cat", cat},
	{(unsigned char *) "echo", echo},
	{(unsigned char *) "locate", locate},
	{(unsigned char *) "rm", rm},
	{(unsigned char *) "rmtag", rmtag},
	{(unsigned char *) "mktag", mktag},
	{(unsigned char *) "mv", mv},
	{(unsigned char *) "cp", cp},
	{(unsigned char *) "cd", cd},
	{(unsigned char *) "detail", detail},
	{(unsigned char *) "top", topProcess},
	{(unsigned char *) "kill", killProc},
	{(unsigned char *) "proc1", fun1},
	{(unsigned char *) "proc2", fun2},
	{(unsigned char *) "changeprio", changeProcPrio},

};



int 
evaluateCommand(unsigned char *cmd, int ground){
	int i, paramFd, status;
	unsigned char realCmd[MAX_COMMAND_SIZE];
	/*static*/ unsigned char opt[MAX_OPT_SIZE], optBuff[MAX_OPT_SIZE];
	unsigned char *p=cmd;
	unsigned char *q;
	int len=strlen((char *)cmd);
	pid_t pid;


	for(i=0; p-cmd<MAX_COMMAND_SIZE-1 && p-cmd<len && *p!=' ' && *p!='\n'; ++i, ++p)
		realCmd[i]=*p;
	if(p-cmd<MAX_COMMAND_SIZE-1)
		realCmd[i]='\0';
	else
		return 0;


	for(i=0 , q=/*p*/++p; p-q<MAX_OPT_SIZE-2 && p-cmd<len && /*agrego */*p !='\n' /**/; ++i, ++p){
		opt[i]=*p;
	}
	if(p-q<MAX_OPT_SIZE-2){
		opt[i++]=' ';
		opt[i]='\0';
	}
	else
		return 0;
	for(i=0; ((unsigned int) i)< sizeof(cmdTable)/sizeof(cmdItem); ++i)
		if(strcmp((char *)cmdTable[i].cmd, (char *)realCmd) == 0 )
			break;
	if( ((unsigned int) i)==sizeof(cmdTable)/sizeof(cmdItem) )
		return 0;
//	cmdTable[i].fn((void *) opt);

	if( (paramFd=open("*:mailBox", O_MAILBOX)) < 0 ){
	    puts((unsigned char *) "ls: Couldn't create mailbox (Unknown error) \n");
	    return 0;
	}

	write(paramFd, opt, strlen(opt)+1);
	/*
	puts((unsigned char *) "Mailbox created with ");
	read(paramFd, optBuff, strlen(opt)+1);
	puts((unsigned char *) optBuff);
	puts((unsigned char *) " fd: ");
	puti(paramFd);
	puts((unsigned char *) " \n");
	*/
	if( (pid=createProcess((char *)realCmd, cmdTable[i].fn, paramFd, ground)) < 0 ){
	    puts((unsigned char *) "Unknown error \n");
	    puts((unsigned char *) getProcessErrorMsg());
	    return 0;
	 }
	if( ground == FOREGROUND){
			waitTty(getFocusTTY());
	}
	else
	    puts((unsigned char *) "Running in background \n");
	close(paramFd);
	return 1;
}
