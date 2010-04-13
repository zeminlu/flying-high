/*
 * File: sysfilesystem.c
 *
 * This file provides the basic system calls to administrate the file-system.
 */

#include "stdio.h"
#include "filesystem.h"
#include "sysfilesystem.h"
#include "string.h"
#include "debug.h"
#include "baseMalloc.h"
#include "memmanager.h"
#include "sysprocess.h"

#define FOPEN_ERROR -2
#define FOPEN_NEGATIVE -1
#define FERROR -1
#define TAG_TO_CREATE FERROR
#define FILE_PAGES 15
extern unsigned char * tty0;
extern unsigned char * tty1;
extern unsigned char * tty2;
extern unsigned char * tty3;
extern unsigned char * tty4;
extern unsigned char * tty5;
	
fileTable_t fileTable;
static tagTable_t tagTable;
openFileTable_t openFiles;
static memArea_t fileMemory;
static void * filePages[FILE_PAGES];

/*
void * tempMallocPage(void){
	static char tempMemory[1024*4*50];
	static int page = 0;
	if(page >= 50)
		return NULL;
	return tempMemory+1024*4*page++;
}*/
	
extern unsigned char screenBufAt[];
static char tempMallocBuffer[10000];
static char * aux = tempMallocBuffer;

void * tempMalloc(int size){
	char * ret;
	if (((tempMallocBuffer + 10000) - (aux + size)) < 0 )
		return NULL;
	ret = aux;
	aux += size;
/*	str_debug(FILESYS_DEBUG, __FUNCTION__, "Se han reservado: ", FALSE);
	int_debug(FILESYS_DEBUG, NULL, aux - tempMallocBuffer, FALSE);*/
/* -- 	str_debug(FILESYS_DEBUG, NULL, "bytes.", TRUE); -- */
	return ret;
}
int getRealFD(int fd){
	pid_t pid;
	int * filesTable, ret;
/*	str_debug(FILESYS_DEBUG, __FUNCTION__, "FD: ", FALSE);
	int_debug(FILESYS_DEBUG, NULL, fd, FALSE);
	str_debug(FILESYS_DEBUG, NULL, " ", TRUE);*/
	if((pid = sysGetpid()) == -1){
/*		str_debug(FILESYS_DEBUG, __FUNCTION__, " El pid dio negativo ", FALSE);*/
		return fd;
	}

	/*_debug();*/
	if((filesTable = getFileTable(pid)) == NULL){
/*		str_debug(FILESYS_DEBUG, __FUNCTION__, " La tabla de archivos dio NULL ", FALSE);*/
		return GENERAL_FYLESYS_ERROR;
	}
	/*_debug();*/
/*	str_debug(FILESYS_DEBUG, __FUNCTION__, "File Table: ", FALSE);
	int_debug(FILESYS_DEBUG, NULL, filesTable[0], FALSE);
	str_debug(FILESYS_DEBUG, NULL, " - ", FALSE);
	int_debug(FILESYS_DEBUG, NULL, filesTable[1], FALSE);
	str_debug(FILESYS_DEBUG, NULL, " - ", FALSE);
	int_debug(FILESYS_DEBUG, NULL, filesTable[2], FALSE);
	str_debug(FILESYS_DEBUG, NULL, " - FD:", FALSE);
	int_debug(FILESYS_DEBUG, NULL, fd, FALSE);
	str_debug(FILESYS_DEBUG, NULL, " - Tabla en FD", FALSE);
	int_debug(FILESYS_DEBUG, NULL, filesTable[fd], FALSE);
	str_debug(FILESYS_DEBUG, NULL, " ", TRUE);*/
	ret = filesTable[fd];
	return ret==-1?FD_FILESYS_ERROR:ret;
}

    int fillProcessFD(int fd){
	    pid_t pid;
	    int * filesTable, i;
	    if((pid = sysGetpid()) == -1)
		    return fd;
	    if((filesTable = getFileTable(pid)) == NULL)
		    return GENERAL_FYLESYS_ERROR;
	    for(i=0; i < MAX_FILES; ++i)
		    if(filesTable[i] == -1)
			    break;
	    filesTable[i] = fd;
	    return i;
    }
	
	void unfillProcessFD(int fd){
	    pid_t pid;
	    int * filesTable;
	    if((pid = sysGetpid()) == -1)
		    return;
	    if((filesTable = getFileTable(pid)) == NULL)
		    return;
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "Se quiere cerrar el fd:  ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, fd, FALSE);
	    str_debug(FILESYS_DEBUG, NULL, "  ", TRUE);
	    filesTable[fd] = -1;
    }
 
    void * tempRealloc(void * p, int newSize, int oldSize){
	    char * aux;
	    if((aux = tempMalloc(newSize)) == NULL)
		    return NULL;
	    memcpy(aux, p, oldSize);
	    return aux;
    }

    void * tempCalloc(int elemSize, int elemQty){
	    char * aux;
	    int i, memQty;
	    memQty = elemSize*elemQty;
	    if((aux = tempMalloc(memQty)) == NULL){
		    /*str_debug(FILESYS_DEBUG, __FUNCTION__, "Choteo calloc\n", FALSE);*/
		    return NULL;
	    }
	    for( i=0; i < memQty; ++i)
		    aux[i]=0;
	    return aux;
    }

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

    int initFileSystem( int maxOpenFiles, int maxFiles, int maxTags){

	    int i;
	    if( maxOpenFiles <= 0 || maxFiles <= 0 || maxTags <= 0)
		    return FALSE;

	    for(i=0; i < FILE_PAGES; ++i){
		    if( (filePages[i] = pageAlloc(TRUE)) == NULL )
			    return FALSE;
	    }
	    fileMemory.address = filePages[0];
	    fileMemory.size = FILE_PAGES * getPageSize();

	    /*str_debug(FILESYS_DEBUG, __FUNCTION__, "El file system se esta inicializando\n", FALSE);*/
	    fileTable.files = tempCalloc(sizeof(fileTableElement_t), maxFiles );
	    fileTable.size = maxFiles;
	    /*str_debug(FILESYS_DEBUG, __FUNCTION__, "Se ha creado la tabla de archivos\n", FALSE);*/
	    tagTable.tags = tempCalloc(sizeof(tagTableElement_t), maxTags );
	    tagTable.size = maxTags;
	    /*str_debug(FILESYS_DEBUG, __FUNCTION__, "Se ha creado la tabla de tags\n", FALSE);*/
	    openFiles.files = tempCalloc(sizeof(openFileTableElement_t), maxOpenFiles );
	    openFiles.size = maxOpenFiles;
	    /*str_debug(FILESYS_DEBUG, __FUNCTION__, "Se ha creado la tabla de archivos abiertos\n", FALSE);*/
	    if((fileTable.files[0].fileName = tempMalloc(strlen("stdin")+1))== NULL)
		    return FALSE;
	    if((fileTable.files[1].fileName = tempMalloc(strlen("stdout")+1))== NULL)
		    return FALSE;
	    if((fileTable.files[2].fileName = tempMalloc(strlen("stdbk")+1))== NULL)
		    return FALSE;
	    strcpy(fileTable.files[0].fileName, "stdin");
    /* -- 	str_debug(FILESYS_DEBUG, __FUNCTION__, fileTable.files[0].fileName, FALSE); -- */
	    fileTable.files[0].tagsIndexes = NULL;
	    fileTable.files[0].amountOfTags = 0;
	    fileTable.files[0].size = 10;
	    fileTable.files[0].address = (void *)0xFFFFFFF0;
	    fileTable.files[0].special = TRUE;
	    

	    strcpy(fileTable.files[1].fileName, "stdout");
    /* -- 	str_debug(FILESYS_DEBUG, __FUNCTION__, fileTable.files[0].fileName, FALSE); -- */
	    fileTable.files[1].tagsIndexes = NULL;
	    fileTable.files[1].amountOfTags = 0;
	    fileTable.files[1].size = 25*80*2+2;
	    fileTable.files[1].address = (void *)0xB8000;
	    fileTable.files[1].special = SPECIAL_SYSTEM;
	    strcpy(fileTable.files[2].fileName, "stdbk");
	    fileTable.files[2].tagsIndexes = NULL;
	    fileTable.files[2].amountOfTags = 0;
	    fileTable.files[2].size = 25*80*2+2;
	    fileTable.files[2].address = &screenBufAt;
	    fileTable.files[2].special = SPECIAL_SYSTEM;

	    openFiles.files[0].fileIndex = 0;
	    openFiles.files[0].open = 1;
	    openFiles.files[0].offset = 0;
	    openFiles.files[1].fileIndex = 1;
	    openFiles.files[1].open = 1;
	    openFiles.files[1].offset = 10;
	    openFiles.files[2].fileIndex = 2;
	    openFiles.files[2].open = 1;
	    openFiles.files[2].offset = 0;
	    

	    if((fileTable.files[3].fileName = tempMalloc(strlen("tty0")+1))== NULL)
		    return FALSE;
	    if((fileTable.files[4].fileName = tempMalloc(strlen("tty1")+1))== NULL)
		    return FALSE;
	    if((fileTable.files[5].fileName = tempMalloc(strlen("tty2")+1))== NULL)
		    return FALSE;
	    if((fileTable.files[6].fileName = tempMalloc(strlen("tty3")+1))== NULL)
		    return FALSE;
	    if((fileTable.files[7].fileName = tempMalloc(strlen("tty4")+1))== NULL)
		    return FALSE;
	    if((fileTable.files[8].fileName = tempMalloc(strlen("tty5")+1))== NULL)
		    return FALSE;

	    strcpy(fileTable.files[3].fileName, "tty0");
	    fileTable.files[3].tagsIndexes = NULL;
	    fileTable.files[3].amountOfTags = 0;
	    fileTable.files[3].size = 25*80*2+2;
	    fileTable.files[3].address = &tty0;
	    fileTable.files[3].special = SPECIAL_SYSTEM;


	    strcpy(fileTable.files[4].fileName, "tty1");
	    fileTable.files[4].tagsIndexes = NULL;
	    fileTable.files[4].amountOfTags = 0;
	    fileTable.files[4].size = 25*80*2+2;
	    fileTable.files[4].address = &tty1;
	    fileTable.files[4].special = SPECIAL_SYSTEM;

	    strcpy(fileTable.files[5].fileName, "tty2");
	    fileTable.files[5].tagsIndexes = NULL;
	    fileTable.files[5].amountOfTags = 0;
	    fileTable.files[5].size = 25*80*2+2;
	    fileTable.files[5].address = &tty2;
	    fileTable.files[5].special = SPECIAL_SYSTEM;


	    strcpy(fileTable.files[6].fileName, "tty3");
	    fileTable.files[6].tagsIndexes = NULL;
	    fileTable.files[6].amountOfTags = 0;
	    fileTable.files[6].size = 25*80*2+2;
	    fileTable.files[6].address = &tty3;
	    fileTable.files[6].special = SPECIAL_SYSTEM;

	    strcpy(fileTable.files[7].fileName, "tty4");
	    fileTable.files[7].tagsIndexes = NULL;
	    fileTable.files[7].amountOfTags = 0;
	    fileTable.files[7].size = 25*80*2+2;
	    fileTable.files[7].address = &tty4;
	    fileTable.files[7].special = SPECIAL_SYSTEM;

	    strcpy(fileTable.files[8].fileName, "tty5");
	    fileTable.files[8].tagsIndexes = NULL;
	    fileTable.files[8].amountOfTags = 0;
	    fileTable.files[8].size = 25*80*2+2;
	    fileTable.files[8].address = &tty5;
	    fileTable.files[8].special = SPECIAL_SYSTEM;
	    
	    openFiles.files[3].fileIndex = 3;
	    openFiles.files[3].open = 1;
	    openFiles.files[3].offset = 0;

	    
	    openFiles.files[4].fileIndex = 4;
	    openFiles.files[4].open = 1;
	    openFiles.files[4].offset = 0;
	    
	    
	    openFiles.files[5].fileIndex = 5;
	    openFiles.files[5].open = 1;
	    openFiles.files[5].offset = 0;
	    
	    openFiles.files[6].fileIndex = 6;
	    openFiles.files[6].open = 1;
	    openFiles.files[6].offset = 0;
	    
	    openFiles.files[7].fileIndex = 7;
	    openFiles.files[7].open = 1;
	    openFiles.files[7].offset = 0;

	    openFiles.files[8].fileIndex = 8;
	    openFiles.files[8].open = 1;
	    openFiles.files[8].offset = 0;
	    
	    fileTable.qty = 9;
	    openFiles.qty = 9;
	    return TRUE;
    }

    int fileExistsOpened(char * file){
	    int i;
	    
	    for(i=0; i < openFiles.size/*qty*/; ++i)
		    if(openFiles.files[i].open > 0)
			    if(strcmp(fileTable.files[openFiles.files[i].fileIndex].fileName, file) == 0)
				    return TRUE;
	    return FALSE;
    }

    int fileExists(char * file){
	    int i;
	    
	    for(i=0; i < fileTable.size/*qty*/; ++i){
		    if(fileTable.files[i].fileName != NULL){
    /* -- 			str_debug(FILESYS_DEBUG, __FUNCTION__, "Comparando: ", FALSE); -- */
    /* -- 			str_debug(FILESYS_DEBUG, __FUNCTION__, fileTable.files[i].fileName, FALSE); -- */
    /* -- 			str_debug(FILESYS_DEBUG, __FUNCTION__, " con ", FALSE); -- */
    /* -- 			str_debug(FILESYS_DEBUG, __FUNCTION__, file, FALSE); -- */
    /* -- 			str_debug(FILESYS_DEBUG, __FUNCTION__, ".\n", FALSE); -- */
		    }
		    if(strcmp(fileTable.files[i].fileName, file) == 0)
			    return i;
	    }
	    return FERROR;
    }

    int tagExists(char * tag){
	    int i;
	    
	    /*str_debug(FILESYS_DEBUG, __FUNCTION__, "Tags en la tabla de tags del sistema:", FALSE);*/
	    /*int_debug(FILESYS_DEBUG, NULL, tagTable.qty, TRUE);*/
	    for(i=0; i < tagTable.size/*qty*/; ++i)
		    if(strcmp(tagTable.tags[i].tagName, tag) == 0)
			    return i;
	    return FERROR;
    }

    int getFreeOpenTableIndex(void){
	    int i;
	    
	    for(i=0; i < openFiles.size; ++i)
		    if( openFiles.files[i].open == 0)
			    return i;
	    return FERROR;
    }

    int getFreeTagTableIndex(void){
	    int i;

	    for(i=0; i < tagTable.size; ++i)
		    if( tagTable.tags[i].tagName == NULL)
			    return i;
	    return FERROR;
    }

    int getFreeFileTableIndex(void){
	    int i;
	    
	    for(i=0; i < fileTable.size; ++i)
		    if(fileTable.files[i].fileName == NULL){
		    /*	str_debug(FILESYS_DEBUG, __FUNCTION__, "Name: ", FALSE);
			    str_debug(FILESYS_DEBUG, __FUNCTION__, fileTable.files[i].fileName, FALSE);
			    puti(i);
			    str_debug(FILESYS_DEBUG, __FUNCTION__, " Fin Name.", FALSE);
		    */	return i;
		    }
	    return FERROR;
    }

    int attachTags(int fileIndex, path_t * path){
	    int i;
	    if(path->tagsQty == 0)
		    return TRUE;
	    if((fileTable.files[fileIndex].tagsIndexes = tempMalloc(path->tagsQty * sizeof(int))) == NULL)
		    return FALSE;
	    for(i = 0; i < path->tagsQty; ++i){
		    fileTable.files[fileIndex].tagsIndexes[i] = path->tagsArray[i].tagID;
		    if((tagTable.tags[path->tagsArray[i].tagID].filesIndexes = tempRealloc(
					    tagTable.tags[path->tagsArray[i].tagID].filesIndexes, 
					    (tagTable.tags[path->tagsArray[i].tagID].amountOfFiles + 1)*sizeof(int), 
					    (tagTable.tags[path->tagsArray[i].tagID].amountOfFiles)*sizeof(int))) == NULL)
			    return FALSE;
		    tagTable.tags[path->tagsArray[i].tagID].filesIndexes[tagTable.tags[path->tagsArray[i].tagID].amountOfFiles++] = fileIndex;
	    }
	    return TRUE;
    }

    int createTags(path_t * path){
	    int i, *ret;
	    for(i=0; i < path->tagsQty; ++i){
		    if(path->tagsArray[i].tagID == TAG_TO_CREATE){
			    ret = sysMkTag(path->tagsArray[i].tagName);
			    if( ret == NULL){
    /* -- 				str_debug(FILESYS_DEBUG, __FUNCTION__, "El retorno de MkTag es NULL \n", FALSE); -- */
				    return FALSE;
			    }
			    path->tagsArray[i].tagID = *ret;
		    }
	    }
	    return TRUE;
    }

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
	* 			- O_MAILBOX: if file doesn't exist, returns negative.
	*
	*		Returns: a file descriptor, or negative if an error ocurred.
	*/
static int mailBox=0;
static char mailBoxName[25] = "#MailBox";
static char mailBoxToStr[15];
    int * sysOpen(path_t * path, char * fileName, int * openFlags){
	    int * i, fileIndex, isMail = FALSE, fileOpenIndex, j, recFlag;
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "sysOpen FILENAME = ", FALSE);
	    str_debug(FILESYS_DEBUG, NULL, fileName, FALSE);
	    str_debug(FILESYS_DEBUG, NULL, " ", TRUE); 
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "sysOpen PATHQTY = ", FALSE);
	   // puti(path->tagsQty);
	    str_debug(FILESYS_DEBUG, NULL, " ", TRUE); 
	    
    /* -- 	str_debug(FILESYS_DEBUG, __FUNCTION__, " \n", FALSE); -- */
	    /*str_debug(FILESYS_DEBUG, __FUNCTION__, "Comenzando a abrir el archivo: ", FALSE);*/
	    /*str_debug(FILESYS_DEBUG, NULL, fileName, TRUE);*/	
	    if((i = tempMalloc(sizeof(int))) == NULL)
		    return NULL;
	    *i = FOPEN_ERROR;
		if (*openFlags == O_MAILBOX) {
		    if((fileName = tempMalloc(strlen(mailBoxName)+15))== NULL)
			    return i;
			strcpy(fileName, mailBoxName);
			intToString(mailBox++, mailBoxToStr);
			strcat(fileName, mailBoxToStr);
			*openFlags = O_CREATE;
			isMail = TRUE;
		}
	    if(path == NULL || fileName == NULL || openFlags == NULL)
		    return i;
	    if((fileOpenIndex = fileExistsOpened(fileName))){
	    /*	for(j=0; j < fileTable.size; ++j){
			    str_debug(FILESYS_DEBUG, __FUNCTION__, "Name: ", FALSE);
			    str_debug(FILESYS_DEBUG, NULL, fileTable.files[j].fileName, FALSE);
			    puti(j);
			    str_debug(FILESYS_DEBUG, NULL, " - ", FALSE);
		    }*/
		    str_debug(FILESYS_DEBUG, __FUNCTION__,  "El archivo ya esta abierto", TRUE);
		    if(*openFlags == O_REPLACE )
			    return i;
		    if(*openFlags == O_EXCL){
			    *i = FOPEN_NEGATIVE;
			    return i;
		    }
		    ++openFiles.files[fileOpenIndex].open;
		    *i = fileOpenIndex;
	    }
	    else if((fileIndex = fileExists(fileName)) != FERROR){
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "El archivo ya existe", TRUE);
		    if(*openFlags == O_REPLACE ){
			    str_debug(FILESYS_DEBUG, __FUNCTION__,  "El flag del archivo existente es REPLACE", TRUE);
		    	if((fileTable.files[fileIndex].fileName = tempMalloc(strlen(fileName)+1))== NULL)
			    	return i;
			    strcpy(path->file.fileName, fileName);
			    path->file.fileID = fileIndex;
			    if(*sysRmFile(path) == FALSE)
				    return i;
			    recFlag = O_CREATE;
			    return sysOpen(path, fileName, &recFlag);
		    }
		    if(*openFlags == O_EXCL){
			    str_debug(FILESYS_DEBUG, __FUNCTION__,  "El flag del archivo existente es EXCL", TRUE);
			    *i = FOPEN_NEGATIVE;
			    return i;
		    }
		    if(openFiles.qty == openFiles.size)
			    return i;
		    fileOpenIndex = getFreeOpenTableIndex();
		    openFiles.files[fileOpenIndex].open = 1;
		    openFiles.files[fileOpenIndex].fileIndex = fileIndex;
		    openFiles.files[fileOpenIndex].offset = 0;
		    openFiles.qty++;
		    *i=fileOpenIndex;
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "El archivo se ha abierto en el indice: ", FALSE);
		    //puti(*i);
		    str_debug(FILESYS_DEBUG, NULL, " ", TRUE);
	    }
	    else{
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "El archivo es nuevo", TRUE);
		    if(*openFlags != O_CREATE)
			    return i;
		    if(fileTable.qty == fileTable.size)
			    return i;
		    fileIndex = getFreeFileTableIndex();
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "El index de la tabla es: ", FALSE);
		    //puti(fileIndex);
		    str_debug(FILESYS_DEBUG, NULL, " ", TRUE);
		    if((fileTable.files[fileIndex].fileName = tempMalloc(strlen(fileName)+1))== NULL)
			    return i;
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "La cantidad de tags es: ", FALSE);
		    int_debug(FILESYS_DEBUG, NULL, path->tagsQty, FALSE);
		    str_debug(FILESYS_DEBUG, NULL, " ", TRUE);
		    if(!createTags(path))
			    return i;
		    if((fileTable.files[fileIndex].tagsIndexes = tempMalloc((path->tagsQty)*sizeof(int))) == NULL){
			    str_debug(FILESYS_DEBUG, __FUNCTION__, "Malloc dio NULL tratando de allocar la tabla de tags para un file", TRUE);
			    return i;
		    }
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "Sus indices y sus tags han sido inicializados.", TRUE);
		    strcpy(fileTable.files[fileIndex].fileName, fileName);
		    fileTable.files[fileIndex].amountOfTags = path->tagsQty;
		    fileTable.files[fileIndex].size = 0;
		    fileTable.files[fileIndex].address = NULL;
			if(isMail)
		    	fileTable.files[fileIndex].special = SPECIAL_MAIL;
			else
				fileTable.files[fileIndex].special = FALSE;
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "Cantidad de Tags en el Path:", FALSE);
		    int_debug(FILESYS_DEBUG, NULL, path->tagsQty, TRUE);
		    for(j = 0; j < path->tagsQty; ++j)
			    fileTable.files[fileIndex].tagsIndexes[j] = path->tagsArray[j].tagID;
		    fileTable.qty++;
		    recFlag = O_EXIST;
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "Comenzando a poner los tags", TRUE);
		    attachTags(fileIndex, path);
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "Comenzando el segundo nivel de la recursividad", TRUE);
		    return sysOpen(path, fileName, &recFlag);
	    }
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "Archivo abierto con respuesta: ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, *i, TRUE);
	    *i = fillProcessFD(*i);
	    return i;
    }

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

    int * sysClose(int * fd) {
	    int * ret, fdMistic=*fd;
	    if((ret = tempMalloc(sizeof(int))) == NULL)
		    return NULL;
	    *ret = FALSE;
	    if( (*fd = getRealFD(*fd)) < 0 )
		    return ret;
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "Se quiere cerrar el fd:  ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, *fd, FALSE);
	    str_debug(FILESYS_DEBUG, NULL, "  -- Sus links:  ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, openFiles.files[*fd].open, TRUE);
	    if( fileTable.files[openFiles.files[*fd].fileIndex].special == SPECIAL_SYSTEM && openFiles.files[*fd].open <= 1){
		    str_debug(FILESYS_DEBUG, __FUNCTION__, "No se cerro", TRUE);
		    return ret;
	    }

	    if(openFiles.files[*fd].open <= 0){
		    return ret;
		}
	    *ret = TRUE;
	    --openFiles.files[*fd].open;
		if( openFiles.files[*fd].open == 0 && fileTable.files[openFiles.files[*fd].fileIndex].special == SPECIAL_MAIL)
			fileTable.files[openFiles.files[*fd].fileIndex].fileName = NULL;
		unfillProcessFD(fdMistic);
	    return ret;
    }


    sysFile_t *
    makeFileArray( int * list, int amount){
	    sysFile_t * filesArray;
	    int i;
	    if( (filesArray = tempMalloc(sizeof(sysFile_t) *( amount + 1))) == NULL)
		    return NULL;
	    for( i = 0; i < amount; ++i){
		    filesArray[i].fileID = list[i];
		    if( (filesArray[i].fileName = tempMalloc(strlen(fileTable.files[list[i]].fileName) + 1)) == NULL)
			    return NULL;
		    strcpy(filesArray[i].fileName, fileTable.files[list[i]].fileName);
	    }
	    filesArray[amount].fileName = NULL;
	    return filesArray;
    }

    tag_t *
    makeTagArray( int * list, int amount){
	    tag_t * tagArray;
	    int i;
	    if( (tagArray = tempMalloc(sizeof(tag_t) * (amount + 1))) == NULL)
		    return NULL;
	    for( i = 0; i < amount; ++i){
		    tagArray[i].tagID = list[i];
		    if( (tagArray[i].tagName = tempMalloc(strlen(tagTable.tags[list[i]].tagName) + 1)) == NULL)
			    return NULL;
		    strcpy(tagArray[i].tagName, tagTable.tags[list[i]].tagName);
	    }
	    tagArray[amount].tagName = NULL;
	    return tagArray;
    }

    int existFile( int index ){
	    return (fileTable.files[index].fileName == NULL)?FALSE:TRUE;
    }

    int existTag( int index ){
	    return (tagTable.tags[index].tagName == NULL)?FALSE:TRUE;
    }
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

    tagView_t * sysReadTag(path_t * path){
	    tagView_t * view;
	    tag_t * tagPath;
	    tagTableElement_t auxTag;
	    fileTableElement_t auxFile;
	    int i, j,g, status, pathAmountOfTags;
	    int auxTags;
	    int files[100];
	    int tags[100];
	    int amountOfFiles = 0;
	    int amountOfTags = 0;

	    if( path == NULL)
		    return NULL;
	    if((view = tempMalloc(sizeof(tagView_t))) == NULL)
		    return NULL;
	    view->tagsArray = NULL;
	    view->filesArray = NULL;
	    if( path->tagsArray == NULL && path->file.fileName == NULL ){
		    for( i = 0; i < fileTable.size; ++i)
    /*			if( fileTable.files[i].fileName != NULL )*/
			    if( existFile(i) )
				    files[amountOfFiles++] = i;
		    view->filesArray = makeFileArray(files, amountOfFiles);
		    for( i = 0; i < tagTable.size ; ++i )
    /*			if( tagTable.tags[i].tagName != NULL ) */
			    if( existTag(i) )
				    tags[amountOfTags++] = i;
		    view->tagsArray = makeTagArray( tags, amountOfTags );
	    }
	    else{

		    tagPath = path->tagsArray;

		    auxTag = tagTable.tags[tagPath[0].tagID]; /* agarro el primer tag */
    /*		for(i = 0; tagPath[i].tagName != NULL; ++i);
    */		pathAmountOfTags = path->tagsQty;

		    //puti(pathAmountOfTags);
		    for( i = 0; i < auxTag.amountOfFiles; ++i ){
			    auxFile = fileTable.files[auxTag.filesIndexes[i]];
			    status = 0;
			    for( j = 0; j < auxFile.amountOfTags; ++j )
				    for( g = 0; g < pathAmountOfTags; ++g)
					    if( auxFile.tagsIndexes[j] == tagPath[g].tagID ){
						    ++status;
						    break;
					    }
			    if( status == pathAmountOfTags )
				    files[amountOfFiles++] = auxTag.filesIndexes[i];/* pongo el numero de entrada de la file que hay q mostrar en el array de files valido*/
		    }
		    view->filesArray = makeFileArray(files, amountOfFiles);
		    for(i = 0; i < amountOfFiles; ++i)
			    for( j = 0 ; j < fileTable.files[view->filesArray[i].fileID].amountOfTags; ++j){
				    status = 1;
				    for(g = 0; g < amountOfTags; ++g)
					    if( tags[g] == fileTable.files[view->filesArray[i].fileID].tagsIndexes[j]){
						    status = 0;
						    break;
					    }
				    if( status == 1){
					    for(g = 0; g < pathAmountOfTags; ++g)
						    if( tagPath[g].tagID == fileTable.files[view->filesArray[i].fileID].tagsIndexes[j]){
							    status = 0;
							    break;
					    }
				    }
    /*						tags[amountOfTags++] = fileTable.files[view->filesArray[i].fileID].tagsIndexes[j];
    */				if(status == 1)
					    tags[amountOfTags++] = fileTable.files[view->filesArray[i].fileID].tagsIndexes[j];
			    }
					    
		    view->tagsArray = makeTagArray(tags, amountOfTags);

	    }
	    return view;
	    
    }

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

    int * sysMkTag(char * tagName){
	    int tagIndex, * ret;
	    if((ret = tempMalloc(sizeof(int))) == NULL)
		    return NULL;
	    *ret = FERROR;
	    if(tagExists(tagName) >=0 || tagTable.qty == tagTable.size)
		    return NULL;
	    if((tagIndex = getFreeTagTableIndex()) == FERROR){
    /* -- 		str_debug(FILESYS_DEBUG, __FUNCTION__, "No hay tags libres\n", FALSE); -- */
		    return NULL;
	    }
	    if((tagTable.tags[tagIndex].tagName = tempMalloc(strlen(tagName)+1)) == NULL){
    /* -- 		str_debug(FILESYS_DEBUG, __FUNCTION__, "No se ha podido allocar espacio para el nombre del tag cuya longitud es ", FALSE); -- */
		    //puti(strlen(tagName)+1);
    /* -- 		str_debug(FILESYS_DEBUG, __FUNCTION__, "\n", FALSE); -- */
		    return NULL;
	    }
	    strcpy(tagTable.tags[tagIndex].tagName, tagName);
	    tagTable.tags[tagIndex].amountOfFiles = 0;
	    tagTable.tags[tagIndex].filesIndexes = NULL;
	    tagTable.qty++;
	    *ret = tagIndex;
	    return ret;
    }
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

    int * sysRmTag(char * tag) {
	    int tagIndex;	
	    int * ret;
	    if((ret = tempMalloc(sizeof(int))) == NULL)
		    return NULL;
	    *ret = FALSE;
	    if(tag == NULL)
		    return ret;
	    if((tagIndex = tagExists(tag))== FERROR)
		    return ret;
	    if (tagTable.tags[tagIndex].amountOfFiles != 0)
		    return ret;
	    *ret = TRUE;
	    tagTable.tags[tagIndex].tagName = NULL;
	    return ret;
    }

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

    int * sysRmFile(path_t * filePath) {
	    int * ret, openFlag;
	    int * temp;
	    int i, j, k;
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "El archivo que se quiere borrar es:", FALSE);
	    str_debug(FILESYS_DEBUG, NULL, filePath->file.fileName, TRUE);
	    str_debug(FILESYS_DEBUG, __FUNCTION__, " -- Su file descriptor: ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, filePath->file.fileID, FALSE);
	    str_debug(FILESYS_DEBUG, NULL, " ", TRUE);
	    if((ret = tempMalloc(sizeof(int))) == NULL)
		    return NULL;
	    *ret = FALSE;
	    if(filePath == NULL)
		    return ret;
	    openFlag = O_EXCL;
	    if(filePath->file.fileID < 0)
		    return ret;
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "El archivo existe", TRUE);
	    if(fileExistsOpened(filePath->file.fileName))
		    return ret;
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "El archivo estra siendo borrado", TRUE);
	    fileTable.files[filePath->file.fileID].fileName = NULL;
	    for( i = 0; i < fileTable.files[filePath->file.fileID].amountOfTags; ++i ){
		    k = 0;
		    temp = tagTable.tags[fileTable.files[filePath->file.fileID].tagsIndexes[i]].filesIndexes;
	    tagTable.tags[fileTable.files[filePath->file.fileID].tagsIndexes[i]].filesIndexes = tempMalloc(sizeof(int) *  (tagTable.tags[fileTable.files[filePath->file.fileID].tagsIndexes[i]].amountOfFiles -1) );
		    for( j = 0; j < tagTable.tags[fileTable.files[filePath->file.fileID].tagsIndexes[i]].amountOfFiles; ++j ){
			    str_debug(FILESYS_DEBUG, __FUNCTION__, "i = ", FALSE);
			    //puti(i);
			    str_debug(FILESYS_DEBUG, __FUNCTION__, " j = ", FALSE);
			   // puti(j);

			    if(  temp[j] != filePath->file.fileID ){
			    tagTable.tags[fileTable.files[filePath->file.fileID].tagsIndexes[i]].filesIndexes[k] = temp[j];
				    ++k;
			    }
		    }
		    --tagTable.tags[fileTable.files[filePath->file.fileID].tagsIndexes[i]].amountOfFiles;
	    }
	    *ret = TRUE;
	    return ret;
    }

    path_t * generatePath( char * path ){
	    char * aux1, *aux2, *aux3;
	    char * tag/* ,*fileName */;
	    int i = 0;
	    int amountOfTags = 0;
	    path_t * retPath;
    /* -- 	str_debug(FILESYS_DEBUG, __FUNCTION__, "generating PATH = \n", FALSE); -- */
    /* -- 	str_debug(FILESYS_DEBUG, __FUNCTION__, path, FALSE); -- */
	    if( (retPath = tempMalloc(sizeof(path_t))) == NULL )
		    return NULL;
	    retPath->tagsArray = NULL;
	    retPath->file.fileName=NULL;
	    if( (aux1 = tempMalloc(sizeof(char) * strlen(path) + 1)) == NULL || (aux2 = tempMalloc(sizeof(char) * strlen(path) + 1)) == NULL )
		    return NULL;
	    strcpy(aux1, &path[0]);
	    strcpy(aux2, &path[0]);
	    tag = strtok(aux1, "*");
	    retPath->tagsQty = amountOfTags ;
    /* if is a file in the root tag: */
	    if(aux2[1] == ':'){
		    retPath->file.fileName = tempMalloc(strlen( &aux2[2]) +1);
		    strcpy(retPath->file.fileName,  &aux2[2]);
		    retPath->file.fileID = fileExists(retPath->file.fileName);
		    return retPath;
	    }
    /* if path is trulated... */
	    if( aux2[1] == '\0' )
		    return retPath;
	    do{
		    ++amountOfTags;
	    }while( (tag= strtok(NULL, "*")) != NULL );
	    /* asigno tagsQty */
	    retPath->tagsQty = amountOfTags ;
	    if( (retPath->tagsArray = tempMalloc( sizeof(tag_t) * (retPath->tagsQty + 1))) == NULL)
		    return  NULL;
	    tag = strtok(aux2,"*");
	    //tag = strtok(NULL,"*");
	    do{
		    retPath->tagsArray[i].tagName = tempMalloc( strlen(tag) + 1);
		    strcpy(retPath->tagsArray[i].tagName, tag);
		    retPath->tagsArray[i].tagID = tagExists(retPath->tagsArray[i].tagName);
		    ++i;
	    }while( (tag = strtok(NULL, "*")) != NULL);
	    retPath->tagsArray[i].tagName = NULL;
	    /* ME FIJO SI VIENE CON UNA FILE AL FINAL */
	    if( (aux3 = tempMalloc( strlen(retPath->tagsArray[i - 1].tagName) + 1)) == NULL)
		    return NULL;
	    strcpy(aux3, retPath->tagsArray[i - 1].tagName);
	    /* SI NO TIENE---> NOMBRE = NULL */
	    if( ( (tag = strtok(aux3, ":")) == NULL )){
		    retPath->file.fileName = NULL;
		    return retPath;
	    }else 
	    /* SI TIENE....*/
	    strcpy(retPath->tagsArray[i - 1].tagName, tag);
	    retPath->tagsArray[i - 1].tagID = tagExists(retPath->tagsArray[i - 1].tagName);
	    tag = strtok( NULL, "\0");
	    if( (retPath->file.fileName = tempMalloc(strlen(tag) + 1)) == NULL)
		    return NULL;
	    strcpy( retPath->file.fileName, tag );
	    retPath->file.fileID = fileExists(retPath->file.fileName);
	    return retPath;
	    
    }

    /*
	* Function:
	*		Description: return  a tag if tagname exists
	*
	*/
    tag_t  getTagByName( char * tagName ){
	    int i;
	    tag_t tag;
	    tag.tagName= NULL;
	    for( i = 0; i < tagTable.size; ++i){
		    if( strcmp(tagTable.tags[i].tagName, tagName) == 0){
			    if( (tag.tagName = tempMalloc( strlen( tagName ) + 1)) == NULL){
				    tag.tagName = NULL;
				    break;
			    }
			    strcpy(tag.tagName, tagTable.tags[i].tagName);
			    tag.tagID = i;
			    break;
		    }
	    }
	    return tag;
    }
    /*
	*	Function int tagNameExists( char * tagName )
	*
	*
	*/

    int tagNameExists( char * tagName ){
	    
	    return (getTagByName( tagName ).tagName)== NULL?FALSE:TRUE;
    }

    /*
	*
	*
	*
	*/
    sysFile_t  getFileByName( char * fileName ){
	    int i;
	    sysFile_t file;
	    file.fileName= NULL;
	    for( i = 0; i < fileTable.size; ++i){
		    if( strcmp(fileTable.files[i].fileName, fileName) == 0){
			    if( (file.fileName = tempMalloc( strlen( fileName ) + 1)) == NULL){
				    file.fileName = NULL;
				    break;
			    }
			    strcpy(file.fileName, fileTable.files[i].fileName);
			    file.fileID = i;
			    break;
		    }
	    }
	    return file;
    }

    tag_t getTagByIndex( int index){
	    tag_t tag;
	    tag.tagName = NULL;
	    if(index >= tagTable.qty)
		    return tag;
	    if( (tag.tagName = tempMalloc( strlen(tagTable.tags[index].tagName) + 1)) == NULL)
		    return tag;
	    strcpy(tag.tagName, tagTable.tags[index].tagName);
	    tag.tagID = index;
	    return tag;	
    }


    /*
	* Function: path_t * sysOpenTag( char * path )
	*
	*      Description: creates a path_t from a string path.
	*
	*     Parameters:
	*        path: path to the tag or file.
	*
	*     Returns: a filesystem path(path_t) to the path.
	*/
    path_t * sysOpenTag( char * path, int mode ) {
	   path_t * retPath;
	   int i = 0;
	   retPath = generatePath( path );
    /* -- 	str_debug(FILESYS_DEBUG, __FUNCTION__, "path generated... \n", FALSE); -- */
	    return retPath;
    }

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
    fileInfo_t * sysGetFileInfo( char * fileName){
	    sysFile_t file;
	    fileInfo_t * fileInfo;
	    int i;

	    file = getFileByName(fileName);
	    if( (fileInfo = tempMalloc( sizeof(fileInfo_t))) == NULL)
		    return NULL;
	    if( (fileInfo->path.tagsArray = tempMalloc( sizeof(tag_t) * (fileTable.files[file.fileID].amountOfTags + 1))) == NULL )
		    return NULL;
	    fileInfo->path.tagsQty = fileTable.files[file.fileID].amountOfTags;
	    fileInfo->fileSize = fileTable.files[file.fileID].size;
	    for( i = 0; i < fileTable.files[file.fileID].amountOfTags; ++i){
		    fileInfo->path.tagsArray[i] = getTagByIndex(fileTable.files[file.fileID].tagsIndexes[i]);
		    
	    
	    }
	    return fileInfo;
    }

    /*
	* Function: int sysLink(int fd)
	*
	* 		Description: replicate an open with this fd.
	*/
    int debugRep=0;
    int *sysLink(int fd){
	    int * ret;
	    //changeTTY(0);
	    str_debug(FILESYS_DEBUG, __FUNCTION__, "Se quiere subir el link del fd mitico ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, fd, FALSE);
	    str_debug(FILESYS_DEBUG, NULL, " ", TRUE);
	    if((ret = tempMalloc(sizeof(int))) == NULL || fd < 0)
		    return NULL;
	    *ret = FALSE;
	    if((fd = getRealFD(fd)) < 0)
		    return ret;
	    str_debug(FILESYS_DEBUG, __FUNCTION__, " El pid ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, sysGetpid(), FALSE);
	    str_debug(FILESYS_DEBUG, NULL, " quiere subir el link del fd ", FALSE);
	    int_debug(FILESYS_DEBUG, NULL, fd, FALSE);
	    str_debug(FILESYS_DEBUG, NULL, " - Sus links:  ", FALSE);
	int_debug(FILESYS_DEBUG, NULL,openFiles.files[fd].open, FALSE);
	str_debug(FILESYS_DEBUG, NULL, " ", TRUE);
	++openFiles.files[fd].open;
	++debugRep;
/*	if(debugRep %10 == 0)
		_debug();*/
	*ret = TRUE;
	return ret;
}
/*
 * Function: int resize(int fd, int n)
 *
 * 		Description: Resize a file, to be possible to write an amount on bytes 
 * 		on it.
 *
 * 		Parameters:
 * 			fd: describe the file to resize
 * 			n: amount of bytes to writo on the file
 *
 * 		Returns: FALSE if the resize failed, TRUE otherwise.
 */

memArea_t getFileMemory( void ){
	return fileMemory; 
}

int resize(int fd, int n){
	openFileTableElement_t * file;
	int newSize;
	file = &(openFiles.files[fd]);
	if( fileTable.files[file->fileIndex].address == NULL ){
		if((fileTable.files[file->fileIndex].address = sysMalloc(n+1, getFileMemory)) == NULL)
			return FALSE;
		fileTable.files[file->fileIndex].size = n+1;
		str_debug(FILESYS_DEBUG, __FUNCTION__, "The malloc succesed, the address is: ", FALSE);
		hexa_debug(FILESYS_DEBUG, NULL, fileTable.files[file->fileIndex].address, TRUE);
	}
	else {
		newSize = n + 1 - (fileTable.files[file->fileIndex].size - file->offset);
		if((fileTable.files[file->fileIndex].address = sysRealloc(fileTable.files[file->fileIndex].address, newSize, getFileMemory)) == NULL)
			return FALSE;
		fileTable.files[file->fileIndex].size = newSize;
		str_debug(FILESYS_DEBUG, __FUNCTION__, "The realloc succesed, the address is: ", FALSE);
		hexa_debug(FILESYS_DEBUG, NULL, fileTable.files[file->fileIndex].address, TRUE);
	}
	return TRUE;
}


