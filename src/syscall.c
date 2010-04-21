/**
 *	File syscall.c
 *
 * 		Brief: The implementation of all system calls
 *
 * 		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */
#include "syscall.h"

extern FILE fileSystem[];
extern process_t *runningProcess;
extern process_t processTable[MAX_PROCESS];

void returnAddress();
void *getKernelHeap();

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

void * _sys_memmap(int isKernel){
	return isKernel ? getKernelHeap() : runningProcess->heap;
}

static void createStackFrame(process_t *process, pfunc_t main, int args) {
	unsigned *esp, espPushedReg;
	
	esp = (unsigned *)(((char *)process->stack) + PAGE_SIZE  - 4);
	
	*esp-- = 0x00000000;
	*esp-- = 0x00000000;
	*esp-- = 0x00000000;
	*esp-- = (unsigned)args;
	*esp-- = (unsigned)exit;
	*esp-- = EFLAGS;
	*esp-- = getCodeSegment();
	espPushedReg = (unsigned)esp;
	*esp-- = (unsigned)main;
	*esp-- = 0xAAAAAAAA;
	*esp-- = 0XCCCCCCCC;
	*esp-- = 0xDDDDDDDD;
	*esp-- = 0xBBBBBBBB;
	*esp-- = espPushedReg;
	*esp-- = 0x11111111;
	*esp-- = 0xEEEEEEEE;
	*esp-- = 0xFFFFFFFF;
	*esp-- = (unsigned)returnAddress;
	*esp =  (unsigned)(esp + 1);
	process->esp = esp - LOCAL_VARIABLES_SPACE;
	process->ebp = esp;

	return;
}

pid_t _sys_create_process(char *name, pfunc_t main, int args, int level) {
	int i, j, nameLen;/*, *linkRet;*/
	process_t *process;
	frame_t *frame;
	

	if ( main == NULL ) {
		return -1;
	}
	
	if ( level != FOREGROUND && level != BACKGROUND ) {
		return -1;
	}

	for ( i = 0; i < MAX_PROCESS; ++i )
		if ( processTable[i].pid == -1 )
			break;
	if ( i == MAX_PROCESS  ) {
		return -1;
	}
	
	process = &processTable[i];
	
	if ( runningProcess->childsQty == MAX_CHILDS ) {
		return -1;
	}
	
	frame = getFrame();
	
	process->stack = (void *)(frame->address + PAGE_SIZE - 1);
	process->heap = (void *)(frame->address + PAGE_SIZE * 2);
	runningProcess->childsQty++;
	
	for ( j = 0; j < MAX_CHILDS; ++j )
		if ( runningProcess->childs[j] == -1 ) {
			runningProcess->childs[j] = i;
			break;
		}
	
	process->pid = i;
	process->ppid = (runningProcess != NULL) ? runningProcess->pid : i;
	process->gid = (runningProcess != NULL) ? runningProcess->gid : i;
	process->tty = (runningProcess != NULL) ? runningProcess->tty : -1;
	process->state = READY;
	process->atomicity = UNATOMIC;
	process->level = level;
	
	/*process->priority = 0;*/
	process->tickCounter = 0;
	process->sleepingPid = -1;
	
	for ( i = 0; i < MAX_CHILDS; ++i )
		process->childs[i] = -1;
	
	process->childsQty = 0;
	
	if ( name != NULL ) {
		memset(process->name, '\0', MAX_PROCESS_NAME + 1);
		if ( (nameLen = strlen(name)) > MAX_PROCESS_NAME )
			nameLen = MAX_PROCESS_NAME;
		memcpy(process->name, name, nameLen);
	}
	
	setFramePresence(frame, TRUE);
	createStackFrame(process, main, args);
	setFramePresence(frame, FALSE);

	return process->pid;
}
