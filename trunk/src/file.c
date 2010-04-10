/*
 *	File: file.c
 *
 */
#include "file.h"
#include "config.h"
#include "system.h"

/*
 * Files Buffer Definition
 */
static char fs_stdin_buffer[SCREEN_SIZE];
static char fs_stdout_buffer[SCREEN_SIZE];
static char fs_in_att_buffer[SCREEN_SIZE];
static char fs_out_att_buffer[SCREEN_SIZE];

/*
 *	File System Table Definition
 */
static int openFiles = 4;

FILE fileSystem[MAX_OPEN_FILES] = {
	{0, fs_stdin_buffer, fs_stdin_buffer,  (_READ | _WRITE), SCREEN_SIZE},
	{1, fs_stdout_buffer, fs_stdout_buffer, (_READ | _WRITE), SCREEN_SIZE},
	{2, fs_in_att_buffer, fs_in_att_buffer, (_READ | _WRITE), SCREEN_SIZE},
	{3, fs_out_att_buffer, fs_out_att_buffer, (_READ | _WRITE), SCREEN_SIZE}
};

/*
 * Public Function File System Access
 */
int getOpenFilesCount ( void ) 
{
	return openFiles;
}
