/**
 *	@file dirShellFunc.h
 *	@brief Defines the api for the basic tag & file shell-related functions
 */

#ifndef DIRSHELLFUNC_H_
#define DIRSHELLFUNC_H_
/*
 *	Includes
 *	========
 */
#include "cmdDispatch.h"
#include "stdlib.h"
#include "basicShellFunc.h"

/*
 *	Defines & Macros
 *	================
 */
/**
 *	@fn clearSpaces(string, index, length)
 *	@brief Increments the given index while the given string contains blank spaces or up to the given length
 *		@param	string the string to parse
 *		@param	index  the index where to begin the parsing operation. It will be incremented while the 
 *				string contains blank spaces or up to the given length
 *		@param	length the limit for the parsing operation. Index will always be < length
 */
#define clearSpaces(string, index, length)			\
	while(isspace((string)[index]) && index<length)		\
		++index;
/**
 *	@fn ifIsHelpShowAndReturn(string, messageHelp) 							
 *	@brief Prints to the standard output the given messageHelp if the given string equals "--help "
 *		@param	string the string to analyze
 *		@param	messageHelp the message to show in case the given string matches "--help "
 */
#define ifIsHelpShowAndReturn(string, messageHelp) 							\
	if(strncmp(string, "--help ", strlen("--help ")) == 0) {	\
			puts((unsigned char *) messageHelp);						\
			return;										\
	}	



/*
 *	Public Functions
 *	================
 */	

/**
 *
 *	@function void cd(unsigned int paramFd)
 *	@brief	The cd function shall change the working directory of the current shell execution environment. Resembles the unix-shell cd 
 *		command with fewer options
 *		SYNOPSIS
 *			cd [OPTION]
 *	@param	opt the string that follows the command
 */	
void cd(int paramFd);

/**
 *
 *	@function void ls(unsigned int paramFd)
 *	@brief list directory contents. Resembles the unix-shell ls command with fewer options.
 *		SYNOPSIS
 *			ls [OPTION]... [FILE]...
 *	@param	opt the string that follows the command
 */	
void ls(int paramFd);

/**
 *	@function void echo(unsigned int paramFd)
 *	@brief  display a line of text.  Resembles the unix-shell echo command with 
 *		fewer functionalities
 *		SYNOPSIS
 *			echo [OPTION]... [STRING]...
 *	@param	opt the string that follows the command
 */

void echo(int paramFd);

/**
 *	@function void rm(unsigned int paramFd)
 *	@brief	remove files or directories.  Resembles the unix-shell rm command with 
 *		fewer functionalities
 *	@param	opt the string that follows the command
 *
 */
void rm(int paramFd);

/**
 *	@function void rmtag(unsigned int paramFd)
 *	@brief	remove empty tags. Resembles the unix-shell rmdir command with 
 *		fewer functionalities
 *		SYNOPSIS
 *		       rmtag [OPTION]... DIRECTORY...
 *	@param	opt the string that follows the command
 */
void rmtag(int paramFd);

/**
 *	@function void mktag(unsigned int paramFd)
 *	@brief	make tags. Resembles the unix-shell mkdir command with 
 *		fewer functionalities
 *		SYNOPSIS
 *			mktag [OPTION] TAG...
 *	@param	opt the string that follows the command
 */
void mktag(int paramFd);

#endif
