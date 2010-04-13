/*
 * File: filesystem.h
 *
 * This file provides the basic system calls to administrate the file-system.
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#define O_CREATE 0
#define O_REPLACE 1
#define O_EXIST 2
#define O_EXCL 3
#define O_MAILBOX 3

typedef struct tag_t {
	char * tagName;
	int tagID;
} tag_t;

typedef struct sysFile_t {
	char * fileName;
	int fileID;
} sysFile_t;

typedef struct path_t {
	tag_t * tagsArray;
	int tagsQty;
	sysFile_t file;
} path_t;

typedef struct tagView_t {
	tag_t * tagsArray;
	sysFile_t * filesArray;
} tagView_t;

typedef struct fileInfo_t {
	path_t path;
	int fileSize;
} fileInfo_t;

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
int initFileSystem( int maxOpenFiles, int maxFiles, int maxTags);
*/
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

int open(char * path, int openFlags);

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

int close(int fd);

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

tagView_t * readTag(path_t path);

/*
 * Function: tag_t mktag(char * tagName)
 *
 * 		Description: Creates a new tag, if tag does not exist.
 * 		
 * 		Parameters:
 * 			tagName: The new tag name.
 *
 * 		Returns: the new tag. If an error ocurred tags name will be NULL.
 */

int mkTag(char * tagName);

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

int rmTag(char * tag);

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

int rmFile(char * filePath);

/*
 * Function: path_t * openTag(const char * path)
 *
 *
 */

path_t * openTag(const char * path);
/*
 *	Function: fileInfo_t * getFileInfo( char * fileName)
 *
 *		Description: get all the information from a file( tags, size, id, etc)
 *
 *		Parameters:
 *				file: the name of the file from wich the information es wanted.
 *
 *		Returns: a pointer to a pathView_t if the file exists, NULL otherwise or error
 *			
 */
fileInfo_t * getFileInfo( char * file);
#endif
