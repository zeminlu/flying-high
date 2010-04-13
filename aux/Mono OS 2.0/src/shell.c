/*
*	File: shell.c
*	Version: 02/06/2009 - 7:13
*	Description: The shell controlls the command-line
*	interface, evaluating commands and executing
*	programs	
*/

/*
*	Includes
*	========
*/
#include "shell.h"
#include "string.h"
#include "stdio.h"
#include "cmdDispatch.h"
#include "filesystem.h"
#include "dirShellFunc.h"
#include "fileShellFunc.h"
#include "process.h"

char cwd[MAX_CWT_SIZE] = "*";
int cwdLength=0;

#define NO_SIGN_MASK 127

void
shellStart(){
	unsigned char line[MAX_LINE];
	int i, j, len,runsInBackground=FALSE, ground=FOREGROUND;

	puts((unsigned char *) "Welcome to monoOS 2.0 - the future is here \n");
	while(1){
		puts((unsigned char *) "monoOS 2.0 : "); 
		puts((unsigned char *) cwd); 
		puts((unsigned char *) ">");
		if( getline(line, sizeof(line), stdin) > 0 ){
			runsInBackground=FALSE;
			len=strlen((char *)line);

			if((line[len-2] & NO_SIGN_MASK) == ('&' & NO_SIGN_MASK)){
				puts((unsigned char *) line+len-2);
				line[len-2]='\0';
				runsInBackground=TRUE;
			}
			if(runsInBackground){
			   ground = BACKGROUND;
			}
			if( !evaluateCommand(line, ground) ){
			 	puts((unsigned char *) "Invalid command \n");
			}
		}
	}
}
