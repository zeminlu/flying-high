/*
 *	File: file.c
 *
 */
#include "file.h"


/* Files Buffer Definition*/
/* 
static char fs_stdin_buffer[SCREEN_SIZE];
static char fs_stdout_buffer[SCREEN_SIZE];
static char fs_in_att_buffer[SCREEN_SIZE];
static char fs_out_att_buffer[SCREEN_SIZE];


 *	File System Table Definition
 
static int openFiles = 4;

FILE fileSystem[MAX_OPEN_FILES] = {
	{0, NULL, NULL,  (_READ | _WRITE), SCREEN_SIZE},
	{1, NULL, NULL, (_READ | _WRITE), SCREEN_SIZE},
	{2, NULL, NULL, (_READ | _WRITE), SCREEN_SIZE},
	{3, NULL, NULL, (_READ | _WRITE), SCREEN_SIZE}
};


 * Public Function File System Access

int getOpenFilesCount ( void ) 
{
	return openFiles;
}
 

typedef struct {
	int fd;
	char * buffer;
	char * ptr;
	int flag;
	size_t bufferSize;
} FILE;

 	char fs_buffers[MAX_TTY][MAX_OPEN_FILES][SCREEN_SIZE];

*/
/*
 *	File System Table Definition
 */
/*FILE fileSystem[MAX_TTY][MAX_FILES];*/
/*
void initFilesystem(){
	int i, j;
	for (i = 0 ; i < MAX_TTY ; ++i){
		for (j = 0 ; j < MAX_OPEN_FILES){
			fileSystem[i][j].fd = j;
			fileSystem[i][j].buffer = fs_buffers[i][i];
			fileSystem[i][j].ptr = fs_buffers[i][i];
			fileSystem[i][j].flag = (_READ | _WRITE);
			fileSystem[i][j].bufferSize = SCREEN_SIZE;
		}
	}
}
*/
