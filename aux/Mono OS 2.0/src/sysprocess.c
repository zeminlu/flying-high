/*
 *	sysprocess.c
 *
 * 		Implements the sysprocess.h interface
 *
 * 		Author: Guido Marucci Blas
 *
 */
#include "sysprocess.h"
#include "string.h"
#include "memmanager.h"
#include "sysfilesystem.h"
#include "debug.h"
#include "kasm.h"
#include "atomic.h"
#include "stdio.h"
#include "tty.h"
#include "pQueue.h"
#include "kernelBridge.h"

#define EFLAGS			0x00200
#define STACK_TRACE_LENGTH 15
#define KILL_EXIT_STATUS 0
#define LOCAL_VARIABLES_SPACE	0x0000001c
#define MILISECONDS_PER_TICK	55

void mtaskRetAddr(void);

typedef struct kernelMemory {
	void *stack;
	void *esp;
	void *ebp;
} kernelMemory_t;

static pQueueADT readyQueues[MAX_PRIORITIES];

static pQueueADT terminatedQueue;

static PROCESS processTable[MAX_PROCESS];

static PROCESS *runningProcess = NULL;

static PROCESS *initProcess = NULL;

static PROCESS *nextProcess = NULL;

static kernelMemory_t kernelMemory;

static int mtaskActivated = FALSE;

static int prioSchedActualProc;

static pid_t ttyRestPlace[MAXAMOUNTOFTTYS];

unsigned tickCounter = 0;

int processErrorCode = PERROR_NO_ERROR;

static const char *processErrorMsg[] = {
	"The action  has been sucsessfully executed.",
	"The function recived bad parameters.",
	"The process could not be created, maximun process capacity has been reached.",
	"Memory could not be allocated.",
	"The Multitasker is allready initiaized.",
	"The init process could not be created.",
	"The parent process has reached it's maximun child capacity.",
	"The given pid is not valid.",
	"The given signal is not valid."
};

static char *stackFrameLabels[] = {
	"ebp: ",
	"multiTaskRetAddr: ",
	"edi: ",
	"esi: ",
	"ebp: ",
	"esp: ",
	"ebx: ",
	"edx: ",
	"ecx: ",
	"eax: ",
	"eip: ",
	"cs: ",
	"eflags: ",
	"exit: ",
	"args: "
};

/*
 *	Private Functions
 */
static void freeProcess(void *p) {
	return;
}

static void *copyProcess(void *p) {
	return p;
}

static void closeAllFiles(PROCESS *process) {
	int i, *files;

	for ( i = 0, files = process->files; i < MAX_FILES; ++i )
		if ( files[i] != -1 )
			sysClose(&files[i]);
}

static void wakeUpProcess(PROCESS *process, int status) {
	//setPagePresence(process->stack, TRUE);
	//setPagePresence(process->heap, TRUE);
	process->waitedStatus = status;
	//setPagePresence(process->stack, FALSE);
	//setPagePresence(process->heap, FALSE);
	process->state = READY;
}

static void alertWaitingProcesses(pid_t pid, int status) {
	int i;
	PROCESS *process;
	if(pid == -1)
		return;
	process = &processTable[processTable[pid].ppid];
	if ( process->state == WAITING_CHILD )
		wakeUpProcess(process, status);

	for ( i = 0; i < MAX_PROCESS; ++i )
		if ( (processTable[i].state == WAITING_PID) && (processTable[i].waitingPid == pid) )
			wakeUpProcess(&processTable[i], status);
	if ( process->sleepingPid != -1 ) {
		for ( i = 0; i < MAXAMOUNTOFTTYS; ++i )
			if ( ttyRestPlace[i] == process->sleepingPid ) {
				ttyRestPlace[i] = -1;
			}
		wakeUpProcess(&processTable[process->sleepingPid], status);
	}

	return;
}

static void terminate(pid_t pid, int status) {
	//pEnqueueElem(terminatedQueue, runningProcess, 1);
	int i;
	pid_t ppid;
	if( pid == -1)
		return;

	ppid = processTable[pid].ppid;
	for( i = 0; i < MAX_CHILDS; ++i)
		if(processTable[ppid].childs[i] == pid)
			processTable[ppid].childs[i] = -1;
	
	processTable[ppid].childsQty--;
	setTtyFocusProcess(processTable[pid].tty, (processTable[pid].ppid > 0) ? processTable[pid].ppid: 0);
	processTable[pid].state = TERMINATED;
	processTable[pid].atomicity = UNATOMIC;
	alertWaitingProcesses(pid, status);
}

static void sigkill(pid_t pid) {
	int i;
	pid_t gid;
	if( pid == -1)
		return;
	if ( pid == 0 )
		for ( i = 0; i < MAX_PROCESS; ++i ) {
			gid = runningProcess->gid;
			if ( processTable[i].gid == gid )
				terminate(i, KILL_EXIT_STATUS);
		}
	else{
		//if( getFocusTTY() == sysGetTty(pid))
		//	return;
		
		for ( i = 0; i < MAX_CHILDS; ++i ) 
			if(processTable[pid].childs[i] != -1)
				terminate(processTable[pid].childs[i], KILL_EXIT_STATUS);
		terminate(pid, KILL_EXIT_STATUS);
	}
	return;
}

static void freeTerminatedProcesses(void) {
	int i, j; 
	for ( i = 0; i < MAX_PROCESS; ++i )
		if ( processTable[i].state == TERMINATED ) {
			processTable[i].pid = -1;
			freePage(processTable[i].stack);
			freePage(processTable[i].heap);
			closeAllFiles(&processTable[i]);
			for ( j = 0; j < MAX_CHILDS; ++j )
				if ( processTable[i].childs[j] != -1 )
					terminate(j, KILL_EXIT_STATUS);
			for ( j = 0; j < MAX_FILES; ++j )
				processTable[i].files[j] = -1;
			processTable[i].state = DEAD;
			//pDequeue(terminatedQueue, 1);
		}
}

#define TIME_SLOT 2 // [times the scheduler is invoked]

static int rrTimeCount;
static int rrActualProc;

static PROCESS *roundRobinSched(void){
    int i;
	++rrTimeCount;
	if( rrTimeCount == TIME_SLOT  || processTable[rrActualProc].state == BLOCKED){
	    rrTimeCount = 0;

	    for( i = rrActualProc + 1;  ;i = (i + 1) % MAX_PROCESS ){
			if( i == rrActualProc ){
			    break;
			}
			if ( processTable[i].pid != -1 &&  processTable[i].state == READY && 
				 processTable[i].pid != processTable[rrActualProc].pid )  {
			    rrActualProc = i; 
			    return &processTable[rrActualProc];
			}
	    }
	}
	return &processTable[rrActualProc];

}

static PROCESS *prioritySched(void){
    int i, maxPrio = -1, candidate=prioSchedActualProc;
    if( processTable[prioSchedActualProc].pid != initProcess->pid){
	   if( processTable[prioSchedActualProc].state != RUNNING)
		  ++processTable[prioSchedActualProc].priority;
	   else if (processTable[prioSchedActualProc].priority > 1)
		  --processTable[prioSchedActualProc].priority;
    }
    for( i = 0; i < MAX_PROCESS; ++i ){
	   if ( processTable[i].pid != -1 &&  processTable[i].state == READY && 
			processTable[i].pid != processTable[prioSchedActualProc].pid )  {
		  if( processTable[i].priority > maxPrio ){
			 maxPrio = processTable[i].priority;
			 candidate = i;
		  }
	   }
    }
    prioSchedActualProc = candidate;
    return &processTable[candidate];


}

static PROCESS *scheduler(void) {
	int i;
	static PROCESS *lastProcess = NULL;

	for ( i = 0; i < MAX_PROCESS; ++i )
		if ( processTable[i].pid != -1 &&  processTable[i].state == READY && 
				(lastProcess == NULL || processTable[i].pid != lastProcess->pid) ) 
			return lastProcess = &processTable[i];

	return runningProcess;
}

static void printStack(PROCESS *process) {
	char *func = "stack-trace"; 
	int i;

	str_debug(MTASK_DEBUG, func, "Process: ", FALSE);
	str_debug(MTASK_DEBUG, NULL, process->name, TRUE);
	str_debug(MTASK_DEBUG, func, "Stack Page Address: ", FALSE);
	hexa_debug(MTASK_DEBUG, NULL, (int)(process->stack), TRUE);
	str_debug(MTASK_DEBUG, func, "esp: ", FALSE);
	hexa_debug(MTASK_DEBUG, NULL, (int)(process->esp), TRUE);
	str_debug(MTASK_DEBUG, func, "ebp: ", FALSE);
	hexa_debug(MTASK_DEBUG, NULL, (int)(process->ebp), TRUE);
	str_debug(MTASK_DEBUG, func, "Stack: ", FALSE);
	for ( i = 0; i < STACK_TRACE_LENGTH; ++i ) {
		str_debug(MTASK_DEBUG, func, stackFrameLabels[i], FALSE);
		hexa_debug(MTASK_DEBUG, NULL, (int)(process->ebp + i*4), FALSE);
		str_debug(MTASK_DEBUG, NULL, "->", FALSE);
		hexa_debug(MTASK_DEBUG, NULL, (int)(*((int *)(process->ebp + i*4))), TRUE);
	}

	return;
}

/*
 * The function creates the following stack frame 
 *	
 *			.			0x00000000
 *			.
 *			.
 *		 EBP------------------------------| multitasker() stack frame	
 *		 multitaskerReturnAddress---------|
 *		 EDI------------------------------| pushad __int_08_hand
 *		 ESI
 *		 EBP
 *		 ESP
 *		 EBX
 *		 EDX
 *		 ECX
 *		 EAX------------------------------|	
 *		 EIP------------------------------| pushed by intel x86 
 *		 CS
 *		 EFLAGS---------------------------|
 *		 exit-----------------------------| fake ret address to exit call
 *		 args----------0x000007FF---------| pointer to args for process' main function
 *
 */
static void createStackFrame(PROCESS *process, pfunc_t main, int args) {
	unsigned *esp, espPushedReg;
	
	pause_debug(MTASK_DEBUG);
	setPagePresence(process->stack, TRUE);
	setPagePresence(process->heap, TRUE);
	esp = process->stack + getPageSize()  - 4;

	/* DEBUG */
	str_debug(MTASK_DEBUG, __FUNCTION__, "%esp->", FALSE);
	hexa_debug(MTASK_DEBUG, NULL, (int)esp, TRUE);
	str_debug(MTASK_DEBUG, __FUNCTION__, "Loading Stack Frame.", TRUE);
	/* END DEBUG */

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
	*esp-- = (unsigned)mtaskRetAddr;
	*esp =  (unsigned)(esp + 1);
	process->esp = esp - LOCAL_VARIABLES_SPACE;
	process->ebp = esp;

	/* DEBUG */
	str_debug(MTASK_DEBUG, __FUNCTION__, "The Stack Frame was successfully created.", TRUE);
	str_debug(MTASK_DEBUG, __FUNCTION__, "%esp->", FALSE);
	hexa_debug(MTASK_DEBUG, NULL, (int)esp, TRUE);
	printStack(process);
	pause_debug(MTASK_DEBUG);	
	/* END DEBUG */
	setPagePresence(process->stack, FALSE);
	setPagePresence(process->heap, FALSE);
	
	return;
}

static int createKernelStack(void) {
	if ( (kernelMemory.stack = pageAlloc(TRUE)) == NULL ) {
		return FALSE;
	}
	kernelMemory.esp = kernelMemory.stack + getPageSize()  - 4;
	kernelMemory.ebp = kernelMemory.esp;
	return TRUE;
}

/*
 *	Public Functions
 */
PROCESS *getProcessTable(void) {
	return processTable;
}

const char *getProcessErrorMsg(void) {
	return processErrorMsg[processErrorCode];	
}

int initMultitasker(pfunc_t init) {
	static int firstTime = TRUE;
	int i, j;
	pid_t pid;

	/* DEBUG */
	str_debug(MTASK_DEBUG, __FUNCTION__, "Initializing multitasker", TRUE);
	/* END DEBUG */

	if ( !firstTime ) {
		processErrorCode = PERROR_MULTITASK_IS_ON;
		return FALSE;
	}

	firstTime = FALSE;
	str_debug(MTASK_DEBUG, __FUNCTION__, "Creating the kernel stack.", TRUE);
	if ( !createKernelStack()  ) {
		processErrorCode = PERROR_MEMORY_ERROR;
		return FALSE;
	}
	for ( i = 0; i < MAX_PROCESS; ++i ) {
		processTable[i].pid = -1;
		processTable[i].state = DEAD;
		for ( j = 0; j < MAX_FILES; ++j )
			processTable[i].files[j] = -1;
	}
	str_debug(MTASK_DEBUG, __FUNCTION__, "Creating the first process.", TRUE);
	if ( (pid = sysCreateProcess("init", init, 0, BACKGROUND)) == -1 ) {
		processErrorCode = PERROR_INIT_FAILED;
		return FALSE;
	}
	str_debug(MTASK_DEBUG, __FUNCTION__, "Creating Queues.", TRUE);
	setKernelHeapPresence(TRUE);
	for ( i = 0; i < MAX_PRIORITIES; ++i )
		if ( (readyQueues[i] = newPQueue(freeProcess, copyProcess)) == NULL  ) {
			processErrorCode = PERROR_MEMORY_ERROR;
			return FALSE;
		}
	if ( (terminatedQueue = newPQueue(freeProcess, copyProcess)) == NULL  ) {
		processErrorCode = PERROR_MEMORY_ERROR;
		return FALSE;
	}
	setKernelHeapPresence(FALSE);
	for ( i = 0; i < MAXAMOUNTOFTTYS; ++i )
		ttyRestPlace[i] = -1;
	initProcess = &processTable[pid];
	initProcess->files[0] = stdin;
	initProcess->files[1] = stdout;
	initProcess->files[2] = stdbk;
	initProcess->files[3] = -1;
	initProcess->priority = 0;
	nextProcess = initProcess;
	mtaskActivated = TRUE;
	
	processErrorCode = PERROR_NO_ERROR;
	return TRUE;
}

void multitasker(void) {

	if ( !mtaskActivated )
		return;

	if ( runningProcess != NULL && runningProcess->atomicity == ATOMIC )
		return;

	nextProcess = /*scheduler()*/ roundRobinSched() /* prioritySched()*/;

	/* DEBUG */
	if ( runningProcess == NULL ) {
		str_debug(MTASK_DEBUG, __FUNCTION__, "Launching the init process from KERNEL BOOT", TRUE);
	}
	/* END DEBUG*/

	if ( nextProcess != runningProcess ) {
		/* This is done when loading the init process for the first time */
		if ( runningProcess != NULL ) {
			if ( runningProcess->state == RUNNING )
				runningProcess->state = READY;
			asm volatile("mov %%esp, %0": "=r"(runningProcess->esp));
			asm volatile("mov %%ebp, %0": "=r"(runningProcess->ebp));
			/* DEBUG */
			str_debug(MTASK_DEBUG, __FUNCTION__,"Changing process ", FALSE);
			str_debug(MTASK_DEBUG, NULL, runningProcess->name, FALSE);
			str_debug(MTASK_DEBUG, NULL, " for ", FALSE);
			str_debug(MTASK_DEBUG, NULL, nextProcess->name, TRUE);
		
			str_debug(MTASK_DEBUG, __FUNCTION__, "Running Process: ", FALSE);
			str_debug(MTASK_DEBUG, NULL, runningProcess->name, FALSE);
			str_debug(MTASK_DEBUG, NULL, " ebp->", FALSE);
			hexa_debug(MTASK_DEBUG, NULL, (int)(runningProcess->ebp), TRUE);
		
			str_debug(MTASK_DEBUG, __FUNCTION__, "Next Process: ", FALSE);
			str_debug(MTASK_DEBUG, NULL, nextProcess->name, FALSE);
			str_debug(MTASK_DEBUG, NULL, " ebp->", FALSE);
			hexa_debug(MTASK_DEBUG, NULL, (int)(nextProcess->ebp), TRUE);
			printStack(runningProcess);
			pause_debug(MTASK_DEBUG);
			/* END DEBUG*/	
		}
		
		/* Protection */
		setPagePresence(nextProcess->stack, TRUE);
		setPagePresence(nextProcess->heap, TRUE);
		
		
		asm volatile("mov %0, %%esp":: "r"(nextProcess->esp));
		asm volatile("mov %0, %%ebp":: "r"(nextProcess->ebp));
		if ( runningProcess != NULL ) {
			setPagePresence(runningProcess->stack, FALSE);
			setPagePresence(runningProcess->heap, FALSE);
		}
		nextProcess->state = RUNNING;
		/* DEBUG */
		str_debug(MTASK_DEBUG, __FUNCTION__, "The next process' stack.", TRUE);
		printStack(nextProcess);
		pause_debug(MTASK_DEBUG);
		/* END DEBUG */

	} else {
		/* DEBUG */
		str_debug(MTASK_DEBUG, __FUNCTION__, "The scheduler has returned the same process.", TRUE);
		/* END DEBUG */
	}

	freeTerminatedProcesses();
	runningProcess = nextProcess;
	if ( runningProcess->level == FOREGROUND ) 
		setTtyFocusProcess(runningProcess->tty, runningProcess->pid);

	/* DEBUG */
	str_debug(MTASK_DEBUG, __FUNCTION__, "Exiting the multitasker.", TRUE);
	/* END DEBUG */
	return;
}

void yield(void) {
	return;
}

void setProcessAtomicity(pid_t pid, int atomic) {
	return;
}

char *getProcessName(pid_t pid) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return NULL;
	return processTable[pid].name;
}

int getProcessState(pid_t pid) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return NULL;
	return processTable[pid].state;
}

int *getFileTable(pid_t pid) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return NULL;
	return processTable[pid].files;
}

int *getOpenedFilesQty(pid_t pid) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return NULL;
	return &(processTable[pid].openedFiles);
}

void waitTty(tty_t tty) {
	ttyRestPlace[tty] = runningProcess->pid;
	runningProcess->state = BLOCKED;
	multitasker2();
	return;
}

void signalTty(tty_t tty) {
	pid_t pid;
	pid = ttyRestPlace[tty];
	if ( processTable[pid].pid != -1 ) {
		processTable[pid].state = READY;
		ttyRestPlace[tty] = -1;
	}
	return;
}

/* System Calls Impementations */
pid_t sysCreateProcess(char *name, pfunc_t main, int args, int level) {
	int i, j, nameLen, *linkRet;
	PROCESS *process;

	if ( main == NULL ) {
		processErrorCode = PERROR_BAD_PARAM;
		return -1;
	}
	if ( level != FOREGROUND && level != BACKGROUND ) {
		processErrorCode = PERROR_BAD_PARAM;
		return -1;
	}

	for ( i = 0; i < MAX_PROCESS; ++i )
		if ( processTable[i].pid == -1 )
			break;
	if ( i == MAX_PROCESS  ) {
		processErrorCode = PERROR_MAX_PROCESS_REACHED;
		return -1;
	}
	process = &processTable[i];
	if ( runningProcess->childsQty == MAX_CHILDS ) {
		processErrorCode = PERROR_MAX_CHILDS_REACHED;
		return -1;
	}
	if ( (process->stack = pageAlloc(FALSE)) == NULL ) {
		processErrorCode = PERROR_MEMORY_ERROR;
		return -1;
	}
	if ( (process->heap = pageDistinctAlloc(FALSE, process->stack)) == NULL ) {
		freePage(process->stack);
		processErrorCode = PERROR_MEMORY_ERROR;
		return -1;
	}
	runningProcess->childsQty++;
	for ( j = 0; j < MAX_CHILDS; ++j )
		if ( runningProcess->childs[j] == -1 ) {
			runningProcess->childs[j] = i;
			break;
		}

	/* DEBUG */
	str_debug(MTASK_DEBUG, __FUNCTION__, "Creating a process with name ", FALSE);
	str_debug(MTASK_DEBUG, NULL, name, TRUE);
	/* END DEBUG */
	
	/* if runningProcess == NULL -> create the init process */
	process->pid = i;
	process->ppid = (runningProcess)?runningProcess->pid:i;
	process->gid = (runningProcess)?runningProcess->gid:i;
	process->state = READY;
	process->tty = (runningProcess)?runningProcess->tty:-1;
	process->openedFiles = 0;
	process->atomicity = UNATOMIC;
	process->level = level;
	process->priority = 100;
	process->tickCounter = 0;
	process->sleepingPid = -1;
	if ( runningProcess != NULL )	
		for ( i = 0; i < MAX_FILES; ++i ) {
			if ( runningProcess->files[i] != -1 &&
				 (linkRet = sysLink(i)) != NULL &&
				 *linkRet == TRUE ) {
				process->files[i] = runningProcess->files[i];
				/* TODO FREE linkRet */
			} else
				process->files[i] = -1;
		}
	process->openedFiles = runningProcess->openedFiles;
	for ( i = 0; i < MAX_CHILDS; ++i )
		process->childs[i] = -1;
	process->childsQty = 0;
	if ( name != NULL ) {
		memset(process->name, '\0', MAX_PROCESS_NAME + 1);
		if ( (nameLen = strlen(name)) > MAX_PROCESS_NAME )
			nameLen = MAX_PROCESS_NAME;
		memcpy(process->name, name, nameLen);
	}
	
	/* DEBUG */
	str_debug(MTASK_DEBUG, __FUNCTION__, "Creating the process' stack frame at ", FALSE);
	hexa_debug(MTASK_DEBUG, NULL, (int)(process->stack), TRUE);
	/* END DEBUG */
	
	createStackFrame(process, main, args);

	/* DEBUG */
	str_debug(MTASK_DEBUG, __FUNCTION__, "The process called ", FALSE);
	str_debug(MTASK_DEBUG, NULL, process->name, FALSE);
	str_debug(MTASK_DEBUG, NULL, " has been created with pid ", FALSE);
	int_debug(MTASK_DEBUG, NULL, process->pid, TRUE);
	/* END DEBUG */
	//pEnqueueElem(readyQueues[0], process, 1);
	processErrorCode = PERROR_NO_ERROR;
	
	return process->pid;
}

int sysKill(int pid, int signal) {
	
	if ( pid >= MAX_PROCESS || pid < 0 ) {
		processErrorCode = PERROR_INVALID_PID;
		return -1;
	}

	switch (signal) {
		case SIGKILL:
			sigkill(pid);
			break;
		default:
			processErrorCode = PERROR_INVALID_SIGNAL;
			return -1;
	}

	processErrorCode = PERROR_NO_ERROR;
	multitasker2();
	return 0;
}

pid_t sysWait(int *status) {
	runningProcess->state = WAITING_CHILD;
	
	multitasker2();
	
	*status = runningProcess->waitedStatus;
	processErrorCode = PERROR_NO_ERROR;
	return runningProcess->waitingPid;
}

pid_t sysWaitpid(pid_t pid, int *status, int options) {
	if ( pid < 0 || pid >= MAX_PROCESS || status == NULL || processTable[pid].pid == -1 || processTable[pid].ppid != runningProcess->pid )
		return -1;
	processTable[pid].sleepingPid = runningProcess->pid;
	runningProcess->state = WAITING_CHILD;

	multitasker2();
	*status = runningProcess->waitedStatus;
	return pid;
}

void sysExit(int status) {
    int i;
    /*
    puts("exit ");
    puts(runningProcess->name);
    puts("\n");
for(i=0; i< 5; ++i){
    	puts("process name: ");
	puts(processTable[i].name);
	puts(" ");
	puti(i);
	puts(" ");
	puti(processTable[i].state);
    	puts("\n");
    }
*/	signalTty(getFocusTTY());
	if ( runningProcess == initProcess )
		return;
	else 
		terminate(runningProcess->pid, status);
	processErrorCode = PERROR_NO_ERROR;
	
	multitasker2();
	exitKernelSpace(FALSE);
	_Sti();
	while(1);

	return;
}

pid_t sysGetpid(void) {
	return (runningProcess != NULL)?runningProcess->pid:-1;
}

pid_t sysGetppid(void) {
	return (runningProcess != NULL)?runningProcess->ppid:-1;
}

pid_t sysGetpgrp(void) {
	return (runningProcess != NULL)?runningProcess->gid:-1;
}

int sysSetlevel(int level) {
	if ( level != FOREGROUND || level != BACKGROUND || runningProcess == NULL  )
		return FALSE;
	runningProcess->level = level;
	return TRUE;
}

int sysGetlevel() {
	return (runningProcess != NULL)?runningProcess->level:-1;
}

int sysGetpriority() {
	return (runningProcess != NULL)?runningProcess->priority:-1;
}

int sysSetpriority(pid_t pid, int prio ) {
	if ( pid <= 0 || pid >= MAX_PROCESS )
	    return -1;
	if ( processTable[pid].pid == -1)
		return -1;
	if ( prio < 1 )
	    return -1;
	return processTable[pid].priority = prio;
}

unsigned sysTime(void) {
	if (runningProcess == NULL)
		return 0;
	return runningProcess->tickCounter * MILISECONDS_PER_TICK;
}

int sysDup(int fd1) {
	int i;

	if ( fd1 >= MAX_FILES || runningProcess == NULL || 
			runningProcess->openedFiles == MAX_FILES - 1 )
		return -1;
	if ( runningProcess->files[fd1] == -1 )
		return -1;
	for ( i = 0; i < MAX_FILES; ++i )
		if ( runningProcess->files[i] == -1 ) { 
			runningProcess->files[i] = runningProcess->files[fd1];
			//TODO Make the link
			break;
		}
	return fd1;
}

int sysDup2(int fd1, int fd2) {
	int aux;
	if ( fd1 >= MAX_FILES || fd2 >= MAX_FILES || runningProcess == NULL || 
			runningProcess->openedFiles == MAX_FILES - 1 )
		return -1;
	if ( runningProcess->files[fd1] == -1 || runningProcess->files[fd2] == -1 )
		return -1;
	aux = runningProcess->files[fd2];
	runningProcess->files[fd2] = runningProcess->files[fd1];
	runningProcess->files[fd1] = aux;
	return fd1;
}

int sysSetpgid(pid_t pid, pid_t pgid) {
	if ( pid == 0 ) {
		if ( runningProcess == NULL )
			return -1;
		runningProcess->gid = pgid;		
	} else {
		if ( processTable[pid].pid == -1 )
			return -1;
		if ( pgid == 0 ) {	
			if ( runningProcess == NULL )
				return -1;
			processTable[pid].gid = runningProcess->gid;
		}
		processTable[pid].gid = pgid;
	}
	return 0;
}

tty_t sysGetTty(pid_t pid) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return -1;
	if ( processTable[pid].pid == -1 )
		return -1;
	return processTable[pid].tty;
}

tty_t sysSetTty(pid_t pid, tty_t tty) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return -1;
	if ( processTable[pid].pid == -1 )
		return -1;
	processTable[pid].tty = tty;
	processTable[pid].files[stdtty] = stdtty + tty;
	return tty;
}
