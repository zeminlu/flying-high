/**
 *	\file syscall.c
 *
 *		\brief Implementation off all system calls.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "syscall.h"

extern process_t *runningProcess;
extern process_t *nextProcess;
extern process_t *initProcess;
extern process_t processTable[MAX_PROCESS];
extern unsigned int tickCount;

static FILE *rFile = NULL;
static FILE *wFile = NULL;

void forceMultitasker();
void returnAddress();
void *getKernelHeap();

size_t _sys_write ( int fd, char * buffer, size_t n )
{
	FILE file;
	char * fileBufferEnd;
	int i;

	if ( buffer == NULL || n == 0 )
		return -1;
	if ( fd < 0 || fd >= MAX_FILES )
		return -1;

	file = (wFile == NULL) ? *(runningProcess->files[fd]) : *wFile;
	if ( file.flag == 0 || (file.flag & _WRITE) != _WRITE )
		return -1;
	fileBufferEnd = file.buffer + file.bufferSize - 1;
	for ( i = 0; n && file.ptr <= fileBufferEnd; ++i, --n )
		*(file.ptr++) = *buffer++;
	if (wFile == NULL){
		*(runningProcess->files[fd]) = file;
	}else{
		*wFile = file;
	} 

	return i;
}

size_t _sys_read ( int fd, char * buffer, size_t n )
{
	FILE file;
	size_t i;
	int remaining;

	if ( buffer == NULL || n == 0 )
		return -1;
	if ( fd < 0 || fd >= MAX_FILES )
		return -1;

	file = (rFile == NULL) ? *(runningProcess->files[fd]) : *rFile;
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
	if (rFile == NULL){
		*(runningProcess->files[fd]) = file;
	}else{
		*rFile = file;
	}
	
	return i;
}

size_t _sys_fread(FILE *stream, char * buffer, size_t n){
	size_t ret;
	
	rFile = stream;
	
	ret = read(stream->fd, buffer, n);
	
	rFile = NULL;
	
	return ret;
}

size_t _sys_fwrite(FILE *stream, char * buffer, size_t n){
	size_t ret;
	
	wFile = stream;
	
	ret = write(stream->fd, buffer, n);
	
	wFile = NULL;
	
	return ret;
}

void * _sys_memmap(int isKernel){
	return isKernel ? getKernelHeap() : runningProcess->heap;
}

void _sysExit(int status) {
/*	signalTty(getFocusTTY());*/
	if ( runningProcess == initProcess )
		return;
	else 
		terminate(runningProcess->pid, status);
	
	forceMultitasker();
	decreaseKernelDepth();
	_Sti();
	while(1)
		asm volatile("hlt");

	return;
}

static void createStackFrame(process_t *process, pfunc_t main, int args) {
	unsigned *esp, espPushedReg;

	esp = (unsigned *)(((char *)process->stack) + PAGE_SIZE * PAGES_PER_FRAME  - 4);
	
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
	int i, j, nameLen;
	process_t *process;

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
	
	process->sFrame = getFrame();
	setFramePresence(process->sFrame, FALSE);
	process->pFrame = getFrame();
	setFramePresence(process->pFrame, FALSE);
	process->hFrame = getFrame();
	setFramePresence(process->hFrame, FALSE);
	
	process->stack = (void *)(process->sFrame->address);
	process->heap = (void *)(process->hFrame->address);
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
	
	process->priority = 3;
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
	
	setFramePresence(process->sFrame, TRUE);
	createStackFrame(process, main, args);
	setFramePresence(process->sFrame, FALSE);
	
	process->ttyMode = TTY_RAW;
	
	if (process->tty > -1 && runningProcess->level == FOREGROUND){
		sysSetTTYFocusedProcess(process->pid, process->tty);
	}
	
	process->dataUmalloc.asigment = TRUE;
	process->dataUmalloc.mallocMem.allocp = process->heap;
	process->dataUmalloc.mallocMem.address = process->heap;
	process->dataUmalloc.mallocMem.size = PAGES_PER_FRAME * PAGE_SIZE;
	
	return process->pid;
}

tty_t _sys_get_tty(pid_t pid) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return -1;
	if ( processTable[pid].pid == -1 )
		return -1;
	return processTable[pid].tty;
}

tty_t _sys_set_tty(pid_t pid, tty_t tty) {
	
	if ( pid > MAX_PROCESS || pid < 0 )
		return -1;
	if ( processTable[pid].pid == -1 )
		return -1;
	processTable[pid].tty = tty;
	
	return tty;
}

pid_t _sys_get_pid()
{
	return (runningProcess != NULL) ? runningProcess->pid:-1;
}

pid_t _sys_get_ppid(){
	return (runningProcess != NULL) ? runningProcess->ppid:-1;
}

pid_t _sys_wait(int *status) {
	runningProcess->state = WAITING_CHILD;
	
	forceMultitasker();
	*status = runningProcess->waitedStatus;
	
	return runningProcess->waitingPid;
}

int _sys_wait_pid(pid_t pid, int *status) {
	if ( pid < 0 || pid >= MAX_PROCESS || status == NULL || processTable[pid].pid == -1 || processTable[pid].ppid != runningProcess->pid )
		return -1;
	runningProcess->waitingPid = pid;
	runningProcess->state = WAITING_PID;

	forceMultitasker();
	*status = runningProcess->waitedStatus;
	
	return pid;
}

int _sys_kill(int pid) {
	int i;
	pid_t gid;
		
	if ( pid >= MAX_PROCESS || pid < 0 ) {
		return -1;
	}

	if ( pid == 0 )
		for ( i = 0; i < MAX_PROCESS; ++i ) {
			gid = runningProcess->gid;
			if ( processTable[i].gid == gid )
				terminate(i, KILL_EXIT_STATUS);
		}
	else{
		if (processTable[pid].pid > 0){
			for ( i = 0; i < MAX_CHILDS; ++i ) {
				if(processTable[pid].childs[i] != -1)
					terminate(processTable[pid].childs[i], KILL_EXIT_STATUS);
			}
			terminate(pid, KILL_EXIT_STATUS);
		}
		else
			return -1;
	}
	
	forceMultitasker();
	return 0;
}

int _sys_set_tty_mode(pid_t pid, int mode) {
	if ( pid < 0 || pid > MAX_PROCESS || processTable[pid].pid < 0 || (mode != TTY_RAW && mode != TTY_CANONICAL))
		return FALSE;
	processTable[pid].ttyMode = mode;
	return TRUE;
}

int _sys_get_tty_mode(pid_t pid) {
	if ( pid < 0 || pid > MAX_PROCESS || processTable[pid].pid < 0)
		return FALSE;
	
	return processTable[pid].ttyMode;
}

int _sys_set_level(int level) {
	if ( level != FOREGROUND || level != BACKGROUND || runningProcess == NULL  )
		return FALSE;
	runningProcess->level = level;
	return TRUE;
}

int _sys_get_level() {
	return (runningProcess != NULL)?runningProcess->level:-1;
}

int _sys_get_priority() {
	return (runningProcess != NULL)?runningProcess->priority:-1;
}

int _sys_set_priority(pid_t pid, int prio ) {
	if ( pid <= 0 || pid >= MAX_PROCESS )
	    return -1;
	if ( processTable[pid].pid == -1)
		return -1;
	if ( prio < 1 )
	    return -1;
	return processTable[pid].priority = prio;
}

unsigned _sys_time(void) {
	if (runningProcess == NULL)
		return 0;
	return runningProcess->tickCounter * MILISECONDS_PER_TICK;
}

unsigned _sys_uptime(){
	return tickCount;
}

int _sys_set_atomicity(pid_t pid, int atomic){
	if (pid < 0 || pid > MAX_PROCESS || (atomic != ATOMIC && atomic != UNATOMIC)){
		return -1;
	}
	processTable[pid].atomicity = atomic;
	
	return 0;
}