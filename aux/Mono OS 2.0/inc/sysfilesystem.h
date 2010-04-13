/*
 * File: sysfilesystem.h
 *
 * This file provides the basic system calls to administrate the file-system.
 */

#ifndef SYSFILESYSTEM_H_
#define SYSFILESYSTEM_H_
#define GENERAL_FYLESYS_ERROR -1
#define FD_FILESYS_ERROR -2
#define SPECIAL_SYSTEM 1
#define SPECIAL_MAIL 2

#include "filesystem.h"
typedef struct fileTableElement {
	char * fileName;
	int * tagsIndexes;
	int amountOfTags;
	int size;
	void * address;
	int flag;
	int special;
} fileTableElement_t;

typedef struct fileTable {
	int size;
	int qty;
	fileTableElement_t * files;
} fileTable_t;

typedef struct tagTableElement {
	char * tagName;
	int amountOfFiles;
	int * filesIndexes;
}tagTableElement_t;

typedef struct tagTable {
	int size;
	int qty;
	tagTableElement_t * tags;
}tagTable_t;

typedef struct openFileTableElement {
	int fileIndex;
	int open;
	int offset;
}openFileTableElement_t;

typedef struct openFileTable {
	int size;
	int qty;
	openFileTableElement_t * files;
}openFileTable_t;

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

int initFileSystem( int maxOpenFiles, int maxFiles, int maxTags);

/*
 * Function: int sysOpen(path_t * path, char * fileName, int * openFlags)
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

int * sysOpen(path_t * path, char * fileName, int * openFlags);

/*
 * Function: int sysClose(int * fd)
 * 
 * 		Description: Closes the file descripted by the file descriptor. 
 *
 * 		Parameters:
 * 			fd: File descriptor that wants to be close.
 *
 * 		Returns: TRUE if succeded, FALSE if error.
 */ 		

int * sysClose(int * fd);

/*
 * Function: tagView_t sysReadTag(path_t * path)
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

tagView_t * sysReadTag(path_t * path);

/*
 * Function: tag_t sysMkTag(char * tagName)
 *
 * 		Description: Creates a new tag, if tag does not exist.
 * 		
 * 		Parameters:
 * 			tagName: The new tag name.
 *
 * 		Returns: the new tag. If an error ocurred tags name will be NULL.
 */

int * sysMkTag(char * tagName);

/*
 * Function: int sysRmTag(char * tag)
 *
 * 		Description: Removes the tag only if the tag have no entries.
 *
 * 		Parameters:
 * 			tag: tag that wants to be removed.
 *
 * 		Returns TRUE if the tag was removed, FALSE otherwise. 
 */

int * sysRmTag(char * tag);

/*
 * Function: int sysRmFile(path_t * filePath)
 * 		
 * 		Description: Removes a file from file system.
 *
 * 		Parameters:
 * 			path: path to the file that wants to be removed.
 * 	
 * 		Returns: TRUE if the file was removed, FALSE otherwise.
 */

int * sysRmFile(path_t * filePath);
/*
 * Function: path_t * sysOpenTag( char * path )
 *
 * 		Description: creates a path_t from a string path.
 *
 * 		Parameters:
 * 			path: path to the tag or file. 
 *
 * 		Returns: a filesystem path(path_t) to the path.
 */
path_t * sysOpenTag( char * path , int mode); 

void * tempMalloc(int size);
void * tempCalloc(int elemSize, int elemQty);


/*
 *	Function: fileInfo_t * sysGetFileInfo( char * fileName)
 *
 *		Description: get all the information from a file( tags, size, id, etc)
 *
 *		Parameters:
 *				file: the name of the file from wich the information es wanted.
 *
 *		Returns: a pointer to a pathView_t if the file exists, NULL otherwise or error
 *			
 */
fileInfo_t * sysGetFileInfo( char * file);

int getRealFD(int fd);

int resize(int fd, int n);

int *sysLink(int fd);

void * tempRealloc(void * p, int newSize, int oldSize);

#endif
