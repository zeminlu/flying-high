/*
 *	File: silesystem.c
 *
 *
 */

#include "debug.h"
#include "callint80.h"
#include "filesystem.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#define ONLY_FOR_OPEN 1
#define NO_FOR_OPEN 0

/**
 *	Function: int initFileSystem( int maxOpenFiles, int maxFiles, int maxTags)
 *		
 *		Description: Initializates the file system according takeing in account 
 *			the parameters.
 *
 *		Parameters:
 *			maxOpenFiles: max number of simultaneously open files.
 *			maxFiles: Max number of files stored in the file system.
 *			maxTags: Max number of tag stored in the file system.
 *
 *		Returns: TRUE if no error, FALSE if error ocurres
 *
 */
/*
int initFileSystem( int maxOpenFiles, int maxFiles, int maxTags){
}
*/


/*
 * Function: int close(fd_t fd)
 * 
 * 		Description: Closes the file descripted by the file descriptor. 
 *
 * 		Parameters:
 * 			fd: File descriptor that wants to be close.
 *
 * 		Returns: TRUE if succeded, FALSE if error.
 */ 		

int close(int fd){
	return *((int *)callInt80(ID_CLOSE, &fd, NULL, NULL));
}

/*
 * Function: tagView_t readTag(path_t path)
 * 	
 * 		Description: Gives all system-file entries applying the path given as 
 * 			parameter.
 *
 * 		Parameters:
 * 			path: The path from which content is desired.
 *
 * 		Returns: path contents.
 *
 */

tagView_t * readTag(path_t path){
	return (tagView_t *)callInt80(ID_READTAG, &path, NULL, NULL);
	
/*	tagView_t ret;
	return ret;
*/
}


/*
 * Function: int mktag(char * tagName)
 *
 * 		Description: Creates a new tag, if tag does not exist.
 * 		
 * 		Parameters:
 * 			tagName: The new tag name.
 *
 * 		Returns: TRUE if no error, FALSE otherwise.
 */

int mkTag(char * tagName){
	int * ret;
	if( (ret = ((int *)callInt80(ID_MKTAG, tagName, NULL, NULL))) == NULL)
		return FALSE;
//	str_debug(FILESYS_DEBUG, __FUNCTION__, "El tag se ha creado correctamente. Retornando a la shell\n", FALSE);
	return *ret;
}
/*
 * Function: int rmTag(char * tag)
 *
 * 		Description: Removes the tag only if the tag have no entries.
 *
 * 		Parameters:
 * 			tag: tag that wants to be removed.
 *
 * 		Returns TRUE if the tag was removed, FALSE otherwise. 
 */

int rmTag(char * tag){
	return *((int *)callInt80(ID_RMTAG, tag, NULL, NULL));
}

/*
 * Function: int rmFile(path_t filePath)
 * 		
 * 		Description: Removes a file from file system.
 *
 * 		Parameters:
 * 			path: path to the file that wants to be removed.
 * 	
 * 		Returns: TRUE if the file was removed, FALSE otherwise.
 */

int rmFile(char * filePath){
	path_t *ret;
	int mode = 1;
	ret = (path_t *)callInt80(ID_OPENTAG, (void *)filePath, &mode, NULL);
	return *((int *)callInt80(ID_RMFILE, ret, NULL, NULL));
}
/*
 * Function: path_t 
 *
 */
path_t * openTagWrp( const char * path , int mode ){
	path_t *ret;
	ret = 	(path_t *)callInt80(ID_OPENTAG, (void *)path, &mode, NULL);
//	str_debug(FILESYS_DEBUG, __FUNCTION__, "openTagWrp ", FALSE);
//	str_debug(FILESYS_DEBUG, __FUNCTION__,  ret->file.fileName , FALSE);
	return ret;
}

/*
 * Function: path_t openTag(const char * path)
 *
 * CAMBIAR
 */

path_t * openTag(const char * path){
	path_t * ret;
	int i;
	int_debug(FILESYS_DEBUG, __FUNCTION__, sizeof(path_t), TRUE);
	ret = openTagWrp( path, NO_FOR_OPEN);
/*	str_debug(FILESYS_DEBUG, __FUNCTION__, " \n luego de openTagWrp", FALSE);
	str_debug(FILESYS_DEBUG, __FUNCTION__, ret.file.fileName, FALSE);
*/
	if(ret->tagsArray == NULL && ret->file.fileName == NULL)
		return ret;
	for( i = 0; ret->tagsArray[i].tagName != NULL; ++i)
		if(ret->tagsArray[i].tagID < 0)
			return NULL;
	return ret;
/*
	return openTagWrp( path, NO_FOR_OPEN);
*/
}

/*
 * Function: int open(path_t path, char * fileName, int openFlags)
 *
 * 		Decription: This function opens a file, according flags.
 *
 * 		openFlags:
 * 			- O_CREATE: if file doesn't exist, creates it.
 * 			- O_REPLACE: if file exists, replaces it.
 * 			- O_EXCL: if file exist, returns negative.
 * 			- O_EXIST: if file doesn't exist, returns negative.
 *
 *		Returns: a file descriptor, or negative if an error ocurred.
 */

int openWrp(path_t * path, char * fileName, int openFlags){
	int * ret;
	
//	str_debug(FILESYS_DEBUG, __FUNCTION__, "\n calling open with tagsQty = ", FALSE);
//	puti(path->tagsQty);
	ret = (int *)callInt80(ID_OPEN, path, fileName, &openFlags);
//	str_debug(FILESYS_DEBUG, __FUNCTION__, "saliendo del open\n", FALSE);
	return ret==NULL?FALSE:*ret;
}

int open(char * path, int openFlags){
	path_t * pathWrp;
//	str_debug(FILESYS_DEBUG, __FUNCTION__, "calling openTAG \n", FALSE);
	pathWrp = openTagWrp(path, 1);
//	str_debug(FILESYS_DEBUG, __FUNCTION__, "calling openWRP \n", FALSE);
//	str_debug(FILESYS_DEBUG, __FUNCTION__, pathWrp->file.fileName, FALSE);
//	str_debug(FILESYS_DEBUG, __FUNCTION__, "\n calling open with tagsQty = ", FALSE);
//	puti(pathWrp->tagsQty);
	return openWrp(pathWrp, pathWrp->file.fileName, openFlags);
}

fileInfo_t * getFileInfo( char * file){
	return callInt80(ID_FILEINFO, file, NULL, NULL);
}
