#include "stdio.h"
#include "stdlib.h"
#include "cmdDispatch.h"
#include "shell.h"
#include "basicShellFunc.h"
#include "string.h"
/*
 * esto es temporal
 */
extern char cwd[MAX_CWT_SIZE];
extern char cwdLength;
/*
 * end esto es temporal
 */

static void clearLastPiriod(char *string){
    int len=strlen(string);

    if(len > 2 && string[len-1] == '.' && string[len-2] == TAG_SEP_AS_CHAR)
	   string[len-2] = '\0';
    return;
}

/**
 *	@fn int isTagPathValid(char *tagPath)
 *	@brief	Checks if the given tagPath is valid by checking that the last tag added does
 *			not appear in the rest of the tag path
 */
int
isTagPathValid(char *tagPath){
    char tagToCheck[MAX_OPT_SIZE];
    int len=strlen(tagPath), i;


    for(i=len-1; tagPath[i] != TAG_SEP_AS_CHAR; --i);
    if( !cpyToFixString(tagToCheck, tagPath+i, sizeof(tagToCheck), TRUE) )
	   return FALSE;
    if( !catToFixString(tagToCheck, TAG_SEP_AS_STRING, sizeof(tagToCheck), TRUE) )
	   return FALSE;
    if( strstr(tagPath, tagToCheck) == NULL )
	   return TRUE;
    return FALSE;
}


int
isTagNameValid(char *tagName){
    int i, len=strlen(tagName);
    for(i=0; i<len; ++i){
	   if( tagName[i] == FILE_SEP_AS_CHAR || tagName[i] == TAG_SEP_AS_CHAR || isspace(tagName[i] ))
		  return FALSE;
    }
    return TRUE;
}

int
parsePiriods(char *string){
	char *pos, buff[MAX_CWT_SIZE];
	clearLastPiriod(string);
	int i;
	while( (pos=strstr(string, "*.*")) != NULL ){
	    if( (int) sizeof(buff) < pos-string )
		   return FALSE;
	    strncpy(buff, string, pos-string);
	    if( !cpyToFixString(buff, pos+2, sizeof(buff), TRUE) )
		   return FALSE;
	    strcpy(string, buff);
	}
	while( (pos=strstr(string, "..")) != NULL ){
	    if(pos == string + 1){
		   if( !cpyToFixString(buff, string+3, sizeof(buff), TRUE) )
			  return FALSE;
		   strcpy(string, buff);
		   if( (pos=strstr(string, "..")) == NULL ){
				break;
		   }
		}
	    *(pos-1)='\0';
	    for(i=strlen(string)-1; string[i] != TAG_SEP_AS_CHAR; --i);
	    string[i]='\0';
	    if( !cpyToFixString(buff, string, sizeof(buff), TRUE) || !catToFixString(buff, pos+2, sizeof(buff), TRUE))
		   return FALSE;
		strcpy(string, buff);

	}
	if( stringHasChar(buff, '.') )
	    return FALSE;
	return TRUE;

}

int 
buildRealPath(char *string, int maxLengthOfString){
	int i=0, len=strlen(string), ret=TRUE, couldBeFile=TRUE;
	char tmp[/*MAX_OPT_SIZE*/MAX_CWT_SIZE];
	tmp[0]='\0';
	if(string[i]==TAG_SEP_AS_CHAR){
		/* va con ruta absoluta */
		couldBeFile=FALSE;
		for(; i<len; ++i)
			if(string[i]==FILE_SEP_AS_CHAR)
				ret=FALSE;
	}
	else{
		for(; i<len; ++i)
			if(string[i]==':'){
				couldBeFile=FALSE;
				break;
			}
		catToFixString(tmp, cwd, sizeof(tmp), TRUE);
		catToFixString(tmp, string, sizeof(tmp), TRUE);	
		if(strlen(tmp) >= maxLengthOfString)
			return BUILD_REAL_PATH_SIZE_ERROR;
		strcpy(string, tmp);
	}
	if( !parsePiriods(string) )
	    return FALSE;
	return ret;
}


int
buildFileWithContext(char *string, int maxLengthOfString){
	int i=0, len=strlen(string), ret=TRUE;
	char tmp[/*MAX_OPT_SIZE*/MAX_CWT_SIZE];
	tmp[0]='\0';
	if(string[i]==TAG_SEP_AS_CHAR){
		/* va con ruta absoluta */
		for(; i<len; ++i)
			if(string[i]==FILE_SEP_AS_CHAR){
				ret=TRUE;
				break;
			}
	}
	else{
		if( !catToFixString(tmp, cwd, sizeof(tmp), TRUE) )
		    return BUILD_REAL_PATH_SIZE_ERROR;
		 for(i=0; i<strlen(string); ++i)
			   if(string[i]==FILE_SEP_AS_CHAR || string[i]==TAG_SEP_AS_CHAR){
					if( !catToFixString(tmp, string, sizeof(tmp), TRUE) || 
					strlen(tmp) >= maxLengthOfString)
						return BUILD_REAL_PATH_SIZE_ERROR;
					strcpy(string, tmp);
					return ret;
			   }
		if( strlen(tmp) == 1){
		    if(!catToFixString(tmp, FILE_SEP_AS_STRING, sizeof(tmp), TRUE) )
			   return BUILD_REAL_PATH_SIZE_ERROR;
		}
		else{
			tmp[strlen(tmp)-1]=FILE_SEP_AS_CHAR;
		}
		if( !catToFixString(tmp, string, sizeof(tmp), TRUE) || 
		strlen(tmp) >= maxLengthOfString)
			return BUILD_REAL_PATH_SIZE_ERROR;
		strcpy(string, tmp);
	}
	if( !parsePiriods(string) )
	    return FALSE;
	return ret;
}


