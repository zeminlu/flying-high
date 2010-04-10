/**
 *	File syscall.c
 *
 * 		Brief: The implementation of all system calls
 *
 * 		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */
#include "file.h"
#include "string.h"

extern FILE fileSystem[];

size_t _sys_write ( int fd, char * buffer, size_t n )
{
	FILE file;
	char * fileBufferEnd;
	int i;

	if ( buffer == NULL || n == 0 )
		return -1;
	if ( fd < 0 || fd >= MAX_OPEN_FILES )
		return -1;

	file = fileSystem[fd];
	if ( file.flag == 0 || (file.flag & _WRITE) != _WRITE )
		return -1;
	fileBufferEnd = file.buffer + file.bufferSize - 1;
	for ( i = 0; n && file.ptr <= fileBufferEnd; ++i, --n )
		*(file.ptr++) = *buffer++;
	fileSystem[fd] = file;

	return i;
}

size_t _sys_read ( int fd, char * buffer, size_t n )
{
	FILE file;
	size_t i;
	int remaining;

	if ( buffer == NULL || n == 0 )
		return -1;
	if ( fd < 0 || fd >= MAX_OPEN_FILES )
		return -1;

	file = fileSystem[fd];
	if ( file.buffer == file.ptr )
		return -1;
	if ( file.flag == 0 || (file.flag & _READ) != _READ )
		return -1;
	for ( i = 0; (i < n) && ((file.buffer + i) < file.ptr); ++i ) 
		buffer[i] = file.buffer[i];
	if ( i < n )
		file.ptr = file.buffer;
	else
	{
		remaining = (file.ptr - (file.buffer + i));
		memcpy(file.buffer, file.buffer + i, remaining);
		file.ptr = file.buffer + remaining;
	}
	fileSystem[fd] = file;

	return i;
}
