
int findTheFile(char *fileName, char *baseTag);

int printFileInfo(char *fileName);

/**
 *	@function void mv(unsigned int paramFd)
 *	@brief	move files. Resembles the unix-shell mv command with 
 *		fewer functionalities
 *		SYNOPSIS
 *			mv SOURCE DEST 
 *	@param	opt the string that follows the command
 */
void mv(int paramFd);

/**
 *	@function void cp(unsigned int paramFd)
 *	@brief	Copy files. Copy SOURCE to DEST.  Resembles the 
 *		unix-shell cp command with fewer functionalities
 *		SYNOPSIS
 *			cp SOURCE DEST 
 *	@param	opt the string that follows the command
 */
void cp(int paramFd);

void detail(int paramFd);

/**
 *	@function void locate(unsigned int paramFd)
 *	@brief	find files by name
 *		SYNOPSIS
 *			locate [OPTION]... PATTERN...
 */
void locate(int paramFd);

/**
 *	@function void cat(unsigned int paramFd)
 *	@brief concatenate files and print on the standard output. Resembles the unix-shell cat command with 
 *		fewer options.
 *		SYNOPSIS
 *     			 cat [OPTION] [FILE]...
 *	@param	opt the string that follows the command
 */
void cat(int paramFd);


