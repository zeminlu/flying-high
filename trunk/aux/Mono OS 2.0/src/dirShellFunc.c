/**
 *	@file dirShellFunc.c
 *	@brief Implementation of the basic tag & file shell-related functions
 */

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
#include "baseMalloc.h"
extern char cwd[MAX_CWT_SIZE];
extern char cwdLength;
/*
 * end esto es temporal
 */

/*
 *	Defines & Macros
 *	================
 */
/*
 *	Private Functions
 *	================
 */


/**
 *	@fn static void getBackOne(char *cwt, char tagSep)
 *	@brief Modifies the cwt so as to get back one tag. E.g: getBackOne("*cars*girls", '*')="*cars*"
 *		@param	cwt the path of tags to modify
 *		@param	tagSep the tags separator		
 */
static void
getBackOne(char *cwt, char tagSep){
	int i;
	int len=strlen(cwt);
	for(i=len-2; cwt[i]!=tagSep; --i);
	cwt[i+1]='\0';
	cwdLength=i+1;
	return;
}



/**
 * 	@fn static int changeCwtTo(char *newCwd)
 *	@brief	Changes the current working tag to newCwd if possible. If the operation can't be realized it prints
 *		and error message and returns FALSE
 *		@return	FALSE if some error was encountered or TRUE otherwise
 *		@param	newCwd the new current working tag
 */
static int
changeCwtTo(char *newCwd){
	int ret;
	if( (ret=cpyToFixString(cwd, newCwd, sizeof(cwd), TRUE)) == TRUE )
		cwdLength=strlen(newCwd); 
	return ret;
}	

/*
 *	Public Functions
 *	================
 */
/**
 *
 *	@function void cd(unsigned char opt[MAX_OPT_SIZE])
 *	@brief	The cd function shall change the working tag of the current shell execution environment. Resembles the unix-shell cd 
 *		command with fewer options
 *		SYNOPSIS
 *			cd [OPTION]
 *	@param	opt the string that follows the command
 */	
void
cd(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
	char opt[MAX_OPT_SIZE];
	read(paramFd, opt, sizeof(opt));
	int i=0, len=strlen(opt);

	char tagWanted[MAX_CWT_SIZE];	
	tagWanted[0]='\0';	
		
	clearSpaces(opt, i, len);
	if(i==len){
		/* va al home */
		changeCwtTo(TAG_SEP_AS_STRING);
		return;
	}
	if(strncmp(opt+i, ". ", strlen(". ")) == 0 )
		return;
	if(strncmp(opt+i, ".. ", strlen(".. ")) == 0){
		getBackOne(cwd, TAG_SEP_AS_CHAR);
		return;
	}
	strtok(opt+i, " ");
	cpyToFixString(tagWanted, opt+i, sizeof(tagWanted), TRUE);
	if( buildRealPath(tagWanted, sizeof(tagWanted)) <= 0 ){
		puts((unsigned char *) "cd: path not valid \n");
		return;
	}
	if( openTag(tagWanted) == NULL ){ 
	    puts((unsigned char *) "cd: tag not found \n");
	    return;
	}
	if( !isTagPathValid(tagWanted) ){
	    puts((unsigned char *) "cd: tag already on path \n");
	    return;
	}
	if( catToFixString(tagWanted, TAG_SEP_AS_STRING, sizeof(tagWanted), TRUE) )
		changeCwtTo(tagWanted);
	return;
}
/**
 *
 *	@function void ls(unsigned char opt[MAX_OPT_SIZE])
 *	@brief list directory contents. Resembles the unix-shell ls command with fewer options.
 *		SYNOPSIS
 *			ls [OPTION]... [FILE]...
 *	@param	opt the string that follows the command
 */	
void 
ls(/*char opt[MAX_OPT_SIZE]*/int paramFd){
	int i=0, j, couldBeFileName=TRUE, verbose=FALSE;
	char opt[MAX_OPT_SIZE];
	read(paramFd, opt, sizeof(opt));
	int len=strlen(opt);
	char tagWanted[/*MAX_OPT_SIZE*/MAX_CWT_SIZE], *fileName, possibleFile[MAX_CWT_SIZE];
	path_t *path;
	tagView_t *tagEntries;
	fileInfo_t *fileInfo;
	
//	path.file.fileName=NULL;
	fileName=NULL;
	
	tagWanted[0]='\0';

	clearSpaces(opt, i, len);
	if(i == len){
		if(!catToFixString(tagWanted, cwd, sizeof(tagWanted), TRUE))
			return;	
		//strcat(tagWanted, cwd);
		if(strlen(tagWanted) > 1)
			tagWanted[strlen(tagWanted)-1]='\0';
		couldBeFileName=FALSE;
	}
	else{
		if( opt[i] == '-' ){
			ifIsHelpShowAndReturn(opt+i, "Usage: ls [OPTION]... [FILE]... \n"
				"List information about the FILEs (the current directory by "
				"default) \n"
				"Examples: \n"
				"'ls file' list the file if it exists in the current working directory \n"
				"'ls tag1*tag2:file' list the file if it exists in the relative path tag1*tag2 \n"
				"'ls *tag1*tag2:file' list the file if it exists in the absolute path \n" );
			if(strncmp(opt+i, "-l ", strlen("-l ")) == 0){
			    verbose=TRUE;
			    i+=strlen("-l ");
			}
		}
		if( opt[i] == FILE_SEP_AS_CHAR /*':'*/ ){
			puts((unsigned char *) "Tags can't begin with ':' \n" "To list a file use ls file if the idea is to look for it in the current "
			"working directory or use ls *tag1*tag2:file \n");
			return;
		}
		strtok(opt+i, " "); 
		cpyToFixString(tagWanted, opt+i, sizeof(tagWanted), TRUE);
		if( buildRealPath(tagWanted, sizeof(tagWanted)) <= 0 ){
			puts((unsigned char *) "Eerror \n");
			return;
		}
			
	}
	if( (path=openTag(tagWanted)) == NULL || (tagEntries=readTag(*path)) == NULL ){
	    if(!couldBeFileName){
		puts((unsigned char *) "ls: Couldn't list tag contents \n");
		return;
	    }
	    for(i=strlen(tagWanted) - 1; tagWanted[i] != TAG_SEP_AS_CHAR; --i);
	    cpyToFixString(possibleFile, tagWanted+i+1, sizeof(possibleFile), TRUE);
	    tagWanted[i+1]='\0';
	    if( findTheFile(possibleFile, tagWanted) == FALSE ){
		   puts((unsigned char *) "ls: Not found \n");
		   return;
	    }
	    printFileInfo(possibleFile);
	    return;
	}
	puts((unsigned char *) "\n files: \n");
	for(i=0; tagEntries->filesArray[i].fileName != NULL; ++i){
		puts((unsigned char *) tagEntries->filesArray[i].fileName);
		if(verbose){
		    puts((unsigned char *) "con :");
		    puts((unsigned char *) tagEntries->filesArray[i].fileName);
		    puts((unsigned char *) ":");
		   if( (fileInfo=getFileInfo(tagEntries->filesArray[i].fileName)) != NULL ){
			  puts((unsigned char *) "tags: ");
			  for(j=0; fileInfo->path.tagsArray[j].tagName != NULL; ++j){
		  		puts((unsigned char *) fileInfo->path.tagsArray[j].tagName);
		  		puts((unsigned char *) " ");
			  }
			  puts((unsigned char *) "size: ");
			  puti(fileInfo->fileSize);
		   }

		}
		puts((unsigned char *) "\n");
	}
	puts((unsigned char *) "\n tags: \n");
	for(i=0; tagEntries->tagsArray[i].tagName != NULL; ++i){
	    puts((unsigned char *) tagEntries->tagsArray[i].tagName);
	    puts((unsigned char *) " ");
	}	

	puts((unsigned char *) "\n");
	//signalTty(getFocusTTY());
	//while(1);
	return;
}



/**
 *	@function void echo(unsigned char opt[MAX_OPT_SIZE])
 *	@brief  display a line of text.  Resembles the unix-shell echo command with 
 *		fewer functionalities
 *		SYNOPSIS
 *			echo [OPTION]... [STRING]...
 *	@param	opt the string that follows the command
 */
void 
echo(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
   	char opt[MAX_OPT_SIZE], buff[MAX_OPT_SIZE];
	read(paramFd, opt, sizeof(opt));
	int fd, len;
	char *optPoint, buff2[20];
	strcpy(buff, opt);
	if( (optPoint=strtok(buff, " ")) == NULL ){
	    puts((unsigned char *) "echo: Missing parameter \n");
	}
	len=strlen(optPoint);
	buildFileWithContext(buff, MAX_OPT_SIZE);
    fd=open(buff, O_CREATE);
    optPoint=opt + len;

    write(fd, optPoint, strlen(optPoint)+1);
    read(fd, buff2, sizeof(buff2));
    close(fd);
    return;
}

static int
blowAllFilesFromTag(char *tag){
    int i, ret=0;
    path_t *openTagInfo;
    tagView_t *readTagInfo;
    char tagWanted[MAX_CWT_SIZE];
    
    cpyToFixString(tagWanted, "*", sizeof(tagWanted), TRUE);
    catToFixString(tagWanted, tag, sizeof(tagWanted), TRUE);
    puts((unsigned char *) "Blowing all files from tag :");
    puts((unsigned char *) tagWanted);
    puts((unsigned char *) ": \n");
    if( (openTagInfo=openTag(tagWanted)) == NULL || (readTagInfo=readTag(*openTagInfo)) == NULL )
	   return FALSE;
    for(i=0; readTagInfo->filesArray[i].fileName != NULL; ++i){
	   ret+=rmFile(readTagInfo->filesArray[i].fileName);
    }
    return ret;
}

/**
 *	@function void rm(unsigned char opt[MAX_OPT_SIZE])
 *	@brief	remove files or tags.  Resembles the unix-shell rm command with 
 *		fewer functionalities
 *	@param	opt the string that follows the command
 *
 */
void 
rm(/*char opt[MAX_OPT_SIZE]*/ int paramFd){
    int i=0, j;
	char opt[MAX_OPT_SIZE];
	read(paramFd, opt, sizeof(opt));
    int len=strlen(opt);
    char *tag, tagWanted[MAX_CWT_SIZE], buffer[MAX_CWT_SIZE];


    clearSpaces(opt, i, len);
    if(i==len){
	   puts((unsigned char *)"rm: missing operand \n"
			 "Try `rm --help' for more information. \n");
	   return;
    }
    if( opt[i] == '-' ){
	   ifIsHelpShowAndReturn(opt+i, "Usage: rm [OPTION]... FILE...\n"
			 "Remove (unlink) the FILE(s). \n"
			 "By default, rm does not remove tag if this one is not empty.  Use the -blow_tag \n"
			 "option to remove a tag (but not its files) or -blow_all to remove the tag and any file that have the given tag \n");
	   if(strncmp(opt+i, "-blow_tag ", strlen("-blow_tag ")) == 0 ){
		  puts((unsigned char *)"Blowing the tag ");
		  tag=strtok(opt+i+strlen("-blow_tag "), " ");
		  puts((unsigned char *) tag);	
		  puts((unsigned char *) "\n");
		  if( !rmTag(tag) )
			 puts((unsigned char *) "rm: tag couldn't be removed \n");
		  return;
	   }
	   if(strncmp(opt+i, "-blow_all ", strlen("-blow_all ")) == 0 ){
		  puts((unsigned char *)"Blowing the tag \n");
		  tag=strtok(opt+i+strlen("-blow_all "), " ");
		  puts((unsigned char *) tag);	
		  puts((unsigned char *) " and any file with that tag \n");
		  if( blowAllFilesFromTag(tag) )
			 if( !rmTag(tag) )
				puts((unsigned char *) "rm: tag couldn't be removed \n");
		  return;
	   }

    }
    strtok(opt+i, " ");
    cpyToFixString(tagWanted, opt+i, sizeof(tagWanted), TRUE); 
    if( buildRealPath(tagWanted, sizeof(tagWanted)) <= 0 ){
	   puts((unsigned char *) "Eerror \n");
	   return;
    }
    len=strlen(tagWanted);
    for(j=1; j<len; ++j)
	   if(tagWanted[j] == TAG_SEP_AS_CHAR)
		  break;
    tagWanted[strlen(tagWanted)-strlen(opt+i)-1]=FILE_SEP_AS_CHAR;
    if(j==len){
	   if(!cpyToFixString(buffer, TAG_SEP_AS_STRING, sizeof(buffer), TRUE) ||
			 !catToFixString(buffer, tagWanted, sizeof(buffer), TRUE) || !cpyToFixString(tagWanted, buffer, sizeof(tagWanted), TRUE) )
		  return;
    }
    if( !rmFile(tagWanted) )
	   puts((unsigned char *) "rm: The file couldn't be removed \n");
    return;
}
/**
 *	@function void rmtag(unsigned char opt[MAX_OPT_SIZE])
 *	@brief	remove empty tags. Resembles the unix-shell rmdir command with 
 *		fewer functionalities
 *		SYNOPSIS
 *		       rmtag [OPTION]... DIRECTORY...
 *	@param	opt the string that follows the command
 */
void 
rmtag(/*char opt[MAX_OPT_SIZE]*/ int paramFd){

	char opt[MAX_OPT_SIZE];
	read(paramFd, opt, sizeof(opt));
    int i=0, len=strlen(opt);


    char tagWanted[/*MAX_OPT_SIZE*/ MAX_CWT_SIZE];


    clearSpaces(opt, i, len);
    if( i == len ){
	   puts((unsigned char *)"rmtag: missing operand \n"
			 "Try `rmtag --help' for more information. \n");
	   return;
    }
    if( opt[i] == '-' ){
	   ifIsHelpShowAndReturn(opt+i, 
			 "Usage: rmtag [OPTION]... FILE...\n"
			 "Remove the tag(s), if they are empty. \n"
			 "               --help     display this help and exit\n");
    }
    cpyToFixString(tagWanted, "-blow_tag ", sizeof(tagWanted), TRUE);
    if( catToFixString(tagWanted, opt, sizeof(tagWanted), TRUE) )
	   rm(tagWanted);
    return;
}

/**
 *	@function void mktag(unsigned char opt[MAX_OPT_SIZE])
 *	@brief	make tags. Resembles the unix-shell mkdir command with 
 *		fewer functionalities
 *		SYNOPSIS
 *			mktag [OPTION] TAG...
 *	@param	opt the string that follows the command
 */
void 
mktag(/*char opt[MAX_OPT_SIZE]*/ int paramFd){

	char opt[MAX_OPT_SIZE];
	read(paramFd, opt, sizeof(opt));
    int i=0, j, len=strlen(opt);

    int newTag;

    clearSpaces(opt, i, len);
    if(i==len){
	   puts((unsigned char *)"mktag: missing operand \n"
			 "Try `mktag --help' for more information. \n");
	   return;
    }
    if( opt[i] == '-' ){
	   ifIsHelpShowAndReturn(opt+i,
			 "Usage: mktag [OPTION] TAG... \n"
			 "Create the tag(s), if they do not already exist.\n"
			 "               --help     display this help and exit\n");
    }
    strtok(opt+i, " ");
    len=strlen(opt+i);
    if(!isTagNameValid(opt+i)){
		  puts((unsigned char *) "mktag: Invalid tag, the tag name can't neither contain *, : nor white spaces \n");
		  return;
    }/*
    for(j=0; j<len; ++j)
	   if((opt+i)[j]=='*' || (opt+i)[j]==' ' || (opt+i)[j]=='\t'){
		  puts((unsigned char *) "mktag: Invalid tag, the tag name can't neither contain * nor white spaces \n");
		  return;
	   }
	   */
    newTag=mkTag(opt+i);
    /*
	  if( newTag == FALSE )
	  puts((unsigned char *) "mktag: Couldn't create tag \n"); 
	  */
    return;
}

