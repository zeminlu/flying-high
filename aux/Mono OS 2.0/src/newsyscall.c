/**
 *	File newsyscall.c
 *
 * 		Brief: The implementation of all system calls
 *
 * 		Author: Santiago Bermudez
 */
#include "sysfilesystem.h"
#include "string.h"
#include "stdlib.h"
#include "newsyscall.h"
#include "sysprocess.h"
#include "debug.h"
extern openFileTable_t openFiles;
extern fileTable_t fileTable; 

size_t _sys_writeController( int fd, const void *buf, size_t count){
	int i;
	if( (fd = getRealFD(fd)) < 0 )
		return fd;
	if( fd == stdout ){
		return writeToTTY(fd, buf, count); 
	}
	return _sys_write(fd, buf, count);
}
size_t _sys_video_write( int fd, char * buffer, size_t n){
	if( fd != stdout){
		if( (fd = getRealFD(fd)) < 0 ){
//			_debug();
			return fd;
		}
	}
//	*((char *)0xB8058)= fd + '0';
	return _sys_write(fd, buffer , n);

}
size_t _sys_write ( int fd, char * buffer, size_t n ){
	openFileTableElement_t * file;
	char * fileBufferEnd, *fileBufferStart;
	int i;
//	*((char*)0xB8014) = '0'+fd;
//	*((char*)0xB8012) = '0'+fd;
//	_debug();
	
	
	file = &(openFiles.files[fd]);
	
	if( fileTable.files[file->fileIndex].special != SPECIAL_SYSTEM){
//		_debug();
		if( !resize(fd, n))
			return RESIZE_WRITE_ERROR;
	}
	
//	*((char*)0xB8030) = fd + '0';
//	*((char*)0xB8031) = 1+ '0';
//	_debug();
	if ( buffer == NULL || n == 0 ){
		return -1;
	}
	if ( fd < 0 || fd >= openFiles.size ){
		return -1;
	}

	fileBufferStart = fileTable.files[file->fileIndex].address;
//	*((char *)0xB8050)= fd + '0';
//	*((char *)0xB8058)=fileTable.files[file->fileIndex].fileName[3];
	
	fileBufferEnd =  fileBufferStart + fileTable.files[file->fileIndex].size - 1;
	for ( i = 0; n && (fileBufferStart + file->offset) <= fileBufferEnd; ++i, --n ){
		*(fileBufferStart + file->offset + i) = *buffer++;
	}
	return i;
}
size_t _sys_readController( int fd, const void *buf, size_t count){
	if( (fd = getRealFD(fd)) < 0 )
		return fd;
	str_debug(FILESYS_DEBUG, __FUNCTION__, " El fd a leer es: ", FALSE);
	int_debug(FILESYS_DEBUG, NULL, fd, FALSE);
	str_debug(FILESYS_DEBUG, NULL, "  ", TRUE);
	if( fd == stdin ){
//		puts("Read controller--> SE ENTRO");
//		return readKBString(buf, count);
		return readFromTTY(buf, count);
	}
	return _sys_read(fd, buf, count);
//	puts("Read controller--> NOO SE ENTRO");
}

size_t _sys_read ( int fd, char * buffer, size_t n ){
	openFileTableElement_t * file;
	size_t i;
	char *fileBufferStart;

	if ( buffer == NULL || n == 0 )
		return -1;
	if ( fd < 0 || fd >= openFiles.size )
		return -1;

	file = &(openFiles.files[fd]);
	if( fileTable.files[file->fileIndex].special == SPECIAL_MAIL)
		file->offset = 0;
	fileBufferStart = fileTable.files[file->fileIndex].address;
	str_debug(FILESYS_DEBUG, __FUNCTION__, " El fd a leer es: ", FALSE);
	int_debug(FILESYS_DEBUG, NULL, fd, FALSE);
	str_debug(FILESYS_DEBUG, NULL, " - su contenido: ", FALSE);
	str_debug(FILESYS_DEBUG, NULL, fileBufferStart, TRUE);
	for ( i = 0; (i < n) && ( (file->offset + i) < fileTable.files[file->fileIndex].size); ++i ) 
		buffer[i] = fileBufferStart[file->offset + i];
	file->offset += i;;
	/*
	puts((unsigned char*) "En reaad: ");
	puti(i);
	puts((unsigned char*) "\n ");
	*/

	return i;
}
