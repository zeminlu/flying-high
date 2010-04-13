/*
 *	Includes
 *	========
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "shell.h"
#include "filesystem.h"
#include "sysfilesystem.h"
#include "dirShellFunc.h"
#include "fileShellFunc.h"
#include "basicShellFunc.h"

/*
 * esto es temporal
 */
extern char cwd[MAX_CWT_SIZE];
extern char cwdLength;
/*
 * end esto es temporal
 */

/**
*/
int
findTheFile(char *fileName, char *baseTag){
    path_t *basePath;
    tagView_t *tagData;
    int i;

    if( (basePath=openTag(baseTag)) == NULL ||
		  (tagData=readTag(*basePath)) == NULL ){
	   puts((unsigned char *) "locate: Couldn't open the tag ");
	   return FALSE;
    }
    for(i=0; tagData->filesArray[i].fileName != NULL ; ++i){
	   puts((unsigned char *) tagData->filesArray[i].fileName);
	   if( strcmp(fileName, tagData->filesArray[i].fileName) == 0 )
		  return TRUE;
    }
    return FALSE;
}

int
printFileInfo(char *fileName){
    fileInfo_t *fileInfo;
    int i;
    if( (fileInfo=getFileInfo(fileName)) == NULL )
	   return FALSE;
    puts((unsigned char *) fileName);
    puts((unsigned char *) "   size: ");
    puti(fileInfo->fileSize);
    puts((unsigned char *) "   tags: ");
    for(i=0; fileInfo->path.tagsArray[i].tagName != NULL; ++i){
	   puts((unsigned char *) fileInfo->path.tagsArray[i].tagName );
	   puts((unsigned char *) " ");
    }
    puts((unsigned char *) "\n");	
    return TRUE;
}



/**
 *	@function void mv(unsigned char opt[MAX_OPT_SIZE])
 *	@brief	move files. Resembles the unix-shell mv command with 
 *		fewer functionalities
 *		SYNOPSIS
 *			mv SOURCE DEST 
 *	@param	opt the string that follows the command
 */
void 
mv(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
    char opt[MAX_OPT_SIZE]; 
    read(paramFd, opt, sizeof(opt));   
    int i=0;
    int len=strlen(opt);


    clearSpaces(opt, i, len);

    if(i==len){
	   puts((unsigned char *)"mv: missing file operand \n"
			 "Try `mv --help' for more information. \n");
	   return;
    }
    if( opt[i] == '-' ){
	   ifIsHelpShowAndReturn(opt+i,
			 "Usage: mv [OPTION]... [-T] SOURCE DEST \n"
			 " or:  mv [OPTION]... SOURCE... DIRECTORY \n"
			 "               --help     display this help and exit\n");
    }
}

/**
 *	@function void cp(unsigned char opt[MAX_OPT_SIZE])
 *	@brief	Copy files. Copy SOURCE to DEST.  Resembles the 
 *		unix-shell cp command with fewer functionalities
 *		SYNOPSIS
 *			cp SOURCE DEST 
 *	@param	opt the string that follows the command
 */
void 
cp(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
    char opt[MAX_OPT_SIZE], buff[MAX_LINE]; 
    read(paramFd, opt, sizeof(opt));   
    int i=0, len=strlen(opt), srcFd, dstFd;
    char src[/*MAX_OPT_SIZE*/ MAX_CWT_SIZE], dst[/*MAX_OPT_SIZE*/ MAX_CWT_SIZE];

    clearSpaces(opt, i, len);
    if(i==len){
	   puts((unsigned char *)"cp: missing file operands \n"
			 "Try `cp --help' for more information. \n");
	   return;
    }
    if( opt[i] == '-' ){
	   ifIsHelpShowAndReturn(opt+i,
			 "Usage: cp SOURCE DEST \n"
			 "               --help     display this help and exit\n"
			 "cp file1 file2	copies file1 to file2 \n");
    }
    strtok(opt, " ");
    cpyToFixString(src, opt, sizeof(src), TRUE);
    buildFileWithContext(src, sizeof(src));
   // buildRealPath(src, sizeof(src));
    cpyToFixString(dst, strtok(NULL, " "), sizeof(dst), TRUE);
   // buildRealPath(dst, sizeof(dst));
   buildFileWithContext(dst, sizeof(dst));
    puts((unsigned char *) "Copying ");
    puts((unsigned char *) src);
    puts((unsigned char *) " to ");
    puts((unsigned char *) dst);
    puts((unsigned char *) "\n");
	srcFd=open(src, O_CREATE );
	dstFd=open(dst, O_CREATE);
	memset(buff, 0, sizeof(buff));
	read(srcFd, buff, sizeof(buff));
	write(dstFd, buff, strlen(buff));
	close(dstFd);
	close(srcFd);
}

void
detail(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
    char opt[MAX_OPT_SIZE]; 
    read(paramFd, opt, sizeof(opt));   
    int i=0;
    fileInfo_t *fileInfo;
    strtok(opt, " ");
    puts((unsigned char *) "Detailing <");
    puts((unsigned char *) opt);
    puts((unsigned char *) ">\n");
    if( (fileInfo=getFileInfo(opt)) != NULL ){
	   puts((unsigned char *) "File found \n");
	   for(i=0; fileInfo->path.tagsArray[i].tagName != NULL; ++i){
		  puts((unsigned char *) fileInfo->path.tagsArray[i].tagName);
		  puts((unsigned char *) " ");
	   }
	   puts((unsigned char *) "size: ");
	   puti(fileInfo->fileSize);
	   puts((unsigned char *) "\n ");
    }
    return;
}

/**
 *	@function void cat(unsigned char opt[MAX_OPT_SIZE])
 *	@brief concatenate files and print on the standard output. Resembles the unix-shell cat command with 
 *		fewer options.
 *		SYNOPSIS
 *     			 cat [OPTION] [FILE]...
 *	@param	opt the string that follows the command
 */
void 
cat(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
    char opt[MAX_OPT_SIZE]; 
    read(paramFd, opt, sizeof(opt));   
    int i=0;
    int len=strlen(opt), toCatFile, readChars;
    unsigned char buff[50], buff2[50], *optPoint;

    clearSpaces(opt, i, len);
    if(i==len){
	   puts((unsigned char *) "cat: Missing file \n");
	   return;
    }
    if( opt[i] == '-' ){
	   ifIsHelpShowAndReturn(opt+i, "Usage: cat [FILE]... \n"
			 "Show file contents to standard output. \n"
			 "               --help     display this help and exit \n"
			 "Examples:\n"
			 "cat pepe        Copy pepe's contents to standard output. \n");
    }
	if( (optPoint=strtok(opt, " ")) == NULL ){
	    puts((unsigned char *) "cat: No files to cat \n");
	    return;
	}
//	do {
	    strcpy(buff, optPoint);
	    buildFileWithContext(buff, sizeof(buff));
   		if( (toCatFile=open(buff, O_EXIST)) < 0){
		    puts((unsigned char *) "cat: File not found \n");
		    return;
		}
		memset(buff2, 0, sizeof(buff2));
		readChars=read(toCatFile, buff2, sizeof(buff2));
		for(i=0; i< readChars || buff2[i] != '\0'; ++i)
		    putchar(buff2[i]);
		puts("\n");
		close(toCatFile);
//	} while( (optPoint=strtok(NULL, " ")) != NULL );

}


/**
 *	@function void locate(unsigned char opt[MAX_OPT_SIZE])
 *	@brief	find files by name. E.g:
 *		locate file -> Looks for the file in the current working tag if present
 *		locate file * -> Looks for the file in the whole directory
 *		locate file *tag1*tag2 -> Looks for the file in the given tags if present
 *		locate -t tag	-> Looks for the given tags and lists its contents if possible
 *		SYNOPSIS
 *			locate [OPTION]... PATTERN...
 */
void 
locate(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
    char opt[MAX_OPT_SIZE]; 
    read(paramFd, opt, sizeof(opt));   
    int i=0, j;
    int len=strlen(opt);
    int fileNameLen;
    char baseTag[/*MAX_OPT_SIZE*/MAX_CWT_SIZE], *tags, *fileName;

    baseTag[0]='\0';

    clearSpaces(opt, i, len);
    if(i==len){
	   puts((unsigned char *)"locate: missing file name to search \n");
	   return;
    }
    if( opt[i] == '-' ){
	   ifIsHelpShowAndReturn(opt+i, "Usage: locate FILE [BASEDIR]...\n"
			 "Locate files \n"
			 "  -t   find the file recursively starting from the BASEDIR \n"
			 "               --help     display this help and exit\n"
			 "Examples:\n locate file -> Looks for the file in the current working tag if present \n"
			 "locate file * -> Looks for the file in the whole filesystem \n"
			 "locate file *tag1*tag2 -> Looks for the file in the given tags if present \n"
			 );
    }
    if( (fileName=strtok(opt, " ")) == NULL ){
	   puts((unsigned char *) "locate: Missing filename \n");
	   puts((unsigned char *) "Usage: locate filename [BASEDIR] \n");
	   return;
    }
    for(j=0, fileNameLen=strlen(fileName); j<fileNameLen; ++j)
	   if(fileName[j] == TAG_SEP_AS_CHAR || fileName[j] == FILE_SEP_AS_CHAR ){
		  puts((unsigned char *) "locate: Invalid filename \n");
		  puts((unsigned char *) "Usage: locate filename [BASEDIR] \n");
		  return;
	   }
    if( (tags=strtok(NULL, " ")) != NULL ){
	   cpyToFixString(baseTag, tags, sizeof(baseTag), TRUE);
	   if( buildRealPath(baseTag, sizeof(baseTag)) <= 0 )
		  puts((unsigned char *) "Eerror \n");
    }
    else{
	   if(!cpyToFixString(baseTag, cwd, sizeof(baseTag), TRUE))
		  return;
	   if(strlen(baseTag) > 1)
	  	 baseTag[strlen(baseTag)-1]='\0';
    }
    puts((unsigned char *) "Looking for file of name: ");
    puts((unsigned char *) fileName);
    puts((unsigned char *) " starting from: ");
    puts((unsigned char *) baseTag);
    puts((unsigned char *) "\n");
    if( findTheFile(fileName, baseTag) == FALSE ){
	   puts((unsigned char *) "locate: File not found \n");
	   return;
    }
    printFileInfo(fileName);
    /*
    fileInfo=getFileInfo(fileName);
    puts((unsigned char *) fileName);
    puts((unsigned char *) "   size: ");
    puti(fileInfo->fileSize);
    puts((unsigned char *) "   tags: ");
    for(i=0; fileInfo->path.tagsArray[i].tagName != NULL; ++i){
	   puts((unsigned char *) fileInfo->path.tagsArray[i].tagName );
	   puts((unsigned char *) " ");
    }
    puts((unsigned char *) "\n");	
    */
    return;
}

