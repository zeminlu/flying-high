/*
*	File: cmdDispatch.h
*	Author: Santiago Perez De Rosso
*	Version: 02/06/09 - 4:29 
*	Description: The command dispatcher evaluates given commands and dispatch
*	the expected functionality if possible
*
*/

#ifndef CMDDISPATCH_H_
#define CMDDISPATCH_H_
/*
*	Defines
*	=======
*/
/*
*	Define: MAX_COMMAND_SIZE
*	Is the maximum length allowed for a command (not considering
*	the possible extra options). The length of the string 
*	up to the first blank should be shorter than this define.
*/
#define MAX_COMMAND_SIZE 20

/*
*	Define: MAX_OPT_SIZE
*	Is the maximum length allowed for the "options" part of a command.
*	This means that the "option part"( from the first blank to the end
*	of the string) should be shorter tan this define.
*/
#define MAX_OPT_SIZE 150 

/*
*
*	Function: evaluateCommand
*	Description: Evaluates the given command and dispatch the 
*	expected functionality if possible.
*	Parameters:
*		cmd: the command to analyze
*	Return:	1 if the command was valid or 0 otherwise
*/
int evaluateCommand(unsigned char *cmd, int ground);

#endif
