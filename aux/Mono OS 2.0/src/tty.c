/*
 *
 *	ttydriver.c
 *
 *
 *
 */

#include "tty.h"
#include "shell.h"
#include "stdio.h"
#include "filesystem.h"
#include "string.h"
#include "videoDriver.h"
#include "KBDriver.h"
#include "sysfilesystem.h"
#include "debug.h"

typedef struct TTY{
	tty_t  	ttyId;
	int 	stdoutFd;
	pid_t	focusProcess;
	unsigned char KBBuffer[SCREEN_SIZE];
	int		buffSize;
}TTY;

typedef struct sysTTY{
	int		ttyAmount;
	int 	stdoutFd;
	tty_t	focusTTY;
	TTY		ttys[MAXAMOUNTOFTTYS];
}sysTTY;
extern fileTable_t fileTable;

sysTTY ttyTable;
int ttyStatus = FALSE;

int setTtyFocusProcess( tty_t tty, pid_t process){
	if( tty < MAXAMOUNTOFTTYS){
		ttyTable.ttys[tty].focusProcess = process;
		return TRUE;
	}
	return FALSE;
}

int initializeTTY( ){
	int i;
	char buff[7];
	char path[10] = "*dev:tty";
	strcpy(buff, "shell");
	for( i = 0; i < MAXAMOUNTOFTTYS; ++i){
		ttyTable.ttys[i].ttyId = i;
		path[8] = buff[5] = '0' + i;
		path[9] = buff[6] = '\0';
		ttyTable.ttys[i].stdoutFd = (i + stdtty)/*open(path, O_CREATE)*/;
		ttyTable.ttys[i].buffSize = 0;
		//		ttyTable.ttys[i].focusProcess = createProcess(buff, shellStart, NULL);

//		ttyTable.ttys[i].stdoutFd = (i + 2); /* open*/ 
/*		ttyTable.ttys[i].focusProcess = createProcess(buff, shellStart, NULL);
*/		
	}
	ttyTable.ttyAmount = i;
	ttyTable.focusTTY = 0;
	ttyStatus = TRUE;
	return i;
}
extern openFileTable_t openFiles; 

int changeTTY( tty_t next){
	int i = 0;
	if(next == ttyTable.focusTTY)
		return FALSE;
	clearKBBuff();
	ttyTable.focusTTY = ttyTable.ttys[next].ttyId;
//	puts("\n pasting to KEYBOARD = ");
	ttyTable.ttys[ttyTable.focusTTY].KBBuffer[ttyTable.ttys[ttyTable.focusTTY].buffSize] = '\0';
//	puts((ttyTable.ttys[ttyTable.focusTTY]).KBBuffer);
	if(	ttyTable.ttys[ttyTable.focusTTY].buffSize > 0){
		pasteInKB((ttyTable.ttys[ttyTable.focusTTY]).KBBuffer, ttyTable.ttys[ttyTable.focusTTY].buffSize );
	}
	memcpy(fileTable.files[stdout].address ,fileTable.files[ttyTable.ttys[ttyTable.focusTTY].stdoutFd].address, 25*80*2+2);
	openFiles.files[stdout].offset = openFiles.files[ttyTable.ttys[ttyTable.focusTTY].stdoutFd].offset;
/*	puts("New tty: ");
	puti(ttyTable.focusTTY);
	puts("\n");*/
	//signalTty(ttyTable.focusTTY);
//	saveScreen();
//	clearScreen();
/*	loadScreen();
*/
	return TRUE;
}
//extern fileTable_t fileTable;

int  writeToTTY(int fd, const void *buf, size_t count){ 
	int i;
	int aux;
	int pid;
	int *deb;
	openFileTableElement_t auxOpen;
	fileTableElement_t auxFile;
	openFiles.files[stdbk].offset = openFiles.files[ttyTable.ttys[sysGetTty(sysGetpid())].stdoutFd].offset;
		for(i=0; i<count; ++i){
				if(*(((unsigned char *) buf) + i)=='\n'){
					if((ttyTable.focusTTY == sysGetTty(sysGetpid())) || sysGetpid() == -1)  
						enterOnScreen(fd);
					else
						enterOnScreen(stdbk);
				}
				else{
		
					if((ttyTable.focusTTY == sysGetTty(sysGetpid())) || sysGetpid() == -1) 
						videoDriverWrite(fd, ((unsigned char *) buf) + i, 1);
					
					else
						videoDriverWrite( stdbk, ((unsigned char *) buf) + i, 1);
				}
		
		}
		
	return TRUE;
}

int readFromTTY( unsigned char *buf, int count){
	if( ttyTable.focusTTY ==sysGetTty(sysGetpid()) /*TODO: póner la task que me viene */){
		if(ttyTable.ttys[ttyTable.focusTTY].focusProcess != sysGetpid()){
			waitTty(MAXAMOUNTOFTTYS + 2);
		}
		str_debug(WAIT_TTY_DEBUG, __FUNCTION__, "TTY correcta ", TRUE);
		if(!isTTYAbleToRead()){
			str_debug(WAIT_TTY_DEBUG, __FUNCTION__, "El archivo dormira... ", TRUE);
			waitTty(ttyTable.focusTTY);
		}
		str_debug(WAIT_TTY_DEBUG, __FUNCTION__, "El archivo no durmio o desperto... ", TRUE);
		return readKBString(buf,count);
	}else{
		str_debug(WAIT_TTY_DEBUG, __FUNCTION__, "El archivo dormira por inactiva... ", TRUE);
		
		waitTty(sysGetTty(sysGetpid()));
		readFromTTY( buf, count);
		//	puts("tty incativa se despierta \n");
	}
	str_debug(WAIT_TTY_DEBUG, __FUNCTION__, "TTY INcorrecta ", TRUE);
	return 0;
}
int ttyBackSpaceOnScreen(){
	if( ttyTable.ttys[ttyTable.focusTTY].buffSize > 0)
		 ttyTable.ttys[ttyTable.focusTTY].buffSize--;
	return backSpaceOnScreen(stdout);

}

void ttyPutchar(unsigned char c){
	if( c == '\n'){
 		ttyTable.ttys[ttyTable.focusTTY].buffSize = 0;
		putchar(c);
	}else if(/*c ==  0x80 || c == 0x81 || c == 0x82*/ c & 0x80 ){
	//	puts("aprendando Fs");
		changeTTY(c & 0x07);
	}
	else if( ttyTable.ttys[ttyTable.focusTTY].buffSize < SCREEN_SIZE -1){
		ttyTable.ttys[ttyTable.focusTTY].KBBuffer[ttyTable.ttys[ttyTable.focusTTY].buffSize] = c;
		ttyTable.ttys[ttyTable.focusTTY].buffSize++;
		ttyTable.ttys[ttyTable.focusTTY].KBBuffer[ttyTable.ttys[ttyTable.focusTTY].buffSize] = '\0';
		putchar(c);
	}
}

tty_t getFocusTTY(){
	return ttyTable.focusTTY;
}
/*
int copyFromFdToFd(int dest, int  src, size){
	ret = malloc(size);
	read(src, ret, size);
	write(dest, ret, size);
	return TRUE;
}
*/
