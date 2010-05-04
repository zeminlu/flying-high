#include "sysProcess.h"

extern FILE **fileSystem;

process_t processTable[MAX_PROCESS];

process_t *runningProcess = NULL;

process_t *initProcess = NULL;

process_t *nextProcess = NULL;

static int mtActivated = FALSE;

static pid_t ttyRestPlace[MAX_TTY];

/*static pid_t nextPID = 1;*/

int qtyProccessTable = 0;

void chupala(){
	char aux = 'A';	
		
	while(1){
		putchar('A');
	}
	
	return;
}

void puto(){
		
	while(1){
		putchar('B');
	}
	return;
}

int getProccessByPid( pid_t pid )
{
	int i;
	
	/*if( pid == 0 )
	{
		return initProcess;
	}*/
	for( i = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( processTable[i].pid == pid )
			return i;
	}
	return -1;
}

int getProccessByName( char * name )
{
	int i;
	
	if( !strcmp(name, "Idle") )
	{
		return -1;
	}
	for( i = 0 ; i < MAX_PROCESS ; ++i )
	{
		if( !strcmp(processTable[i].name, name) )
			return i;
	}
	return -1;
}



void refresh(){
	refreshTTY();
}

int isMTActivated(){
	return mtActivated;
}

static void freeTerminatedProcesses(void) {
	int i, j; 
	
	for ( i = 0; i < MAX_PROCESS; ++i ){
		if ( processTable[i].state == TERMINATED ) {
			processTable[i].pid = -1;
			freeFrame(processTable[i].sFrame);
			freeFrame(processTable[i].hFrame);
			for ( j = 0; j < MAX_CHILDS; ++j )
				if ( processTable[i].childs[j] != -1 )
					terminate(j, KILL_EXIT_STATUS);			
					processTable[i].state = DEAD;
		}
	}
	
	return;
}

int initMultitasker(pfunc_t init) {
	static int firstTime = TRUE;
	int i, j;
	pid_t pid;

	if ( !firstTime ) {
		return FALSE;
	}
	firstTime = FALSE;
	
	for ( i = 0; i < MAX_PROCESS; ++i ) {
		processTable[i].pid = -1;
		processTable[i].state = DEAD;
		for ( j = 0; j < MAX_FILES; ++j )
			processTable[i].files[j] = NULL;
	}
	
	if ( (pid = _sys_create_process("init", init, 0, BACKGROUND)) == -1 ) {
		return FALSE;
	}
	
	for ( i = 0; i < MAX_TTY; ++i )
		ttyRestPlace[i] = -1;
	initProcess = &processTable[pid];
//	initProcess->files[0] = stdin;
//	initProcess->files[1] = stdout;
	initProcess->priority = 0;
	nextProcess = initProcess;
	mtActivated = TRUE;
	
	return TRUE;
}

void multitasker(void) {

	if (!mtActivated)
		return;
	if ( runningProcess != NULL && runningProcess->atomicity == ATOMIC )
		return;
	nextProcess = (runningProcess == NULL) ? nextProcess : getNextTask() /*roundRobinSched()*/ /* prioritySched()*/;
	if ( nextProcess != runningProcess ) {
		/* This is done when loading the init process for the first time */
		if ( runningProcess != NULL ) {
			if ( runningProcess->state == RUNNING )
				runningProcess->state = READY;
			asm volatile("mov %%esp, %0": "=r"(runningProcess->esp));
			asm volatile("mov %%ebp, %0": "=r"(runningProcess->ebp));
		}
		/* Protection */
		setFramePresence(nextProcess->sFrame, TRUE);
		setFramePresence(nextProcess->hFrame, TRUE);
		asm volatile("mov %0, %%esp":: "r"(nextProcess->esp));
		asm volatile("mov %0, %%ebp":: "r"(nextProcess->ebp));
		if ( runningProcess != NULL ) {
			setFramePresence(runningProcess->sFrame, FALSE);
			setFramePresence(runningProcess->hFrame, FALSE);
		}
		nextProcess->state = RUNNING;
	}else {
	}
	freeTerminatedProcesses();
	runningProcess = nextProcess;
	/*if ( runningProcess->level == FOREGROUND ) 
		setTtyFocusProcess(runningProcess->tty, runningProcess->pid);
	*/
	return;
}

static void wakeUpProcess(process_t *process, int status) {
	process->waitedStatus = status;
	process->state = READY;

	return;
}

pid_t sysWait(int *status) {
	runningProcess->state = WAITING_CHILD;
	forceMultitasker();
	*status = runningProcess->waitedStatus;
	
	return runningProcess->waitingPid;
}

pid_t sysWaitpid(pid_t pid, int *status, int options) {
	if ( pid < 0 || pid >= MAX_PROCESS || status == NULL || processTable[pid].pid == -1 || processTable[pid].ppid != runningProcess->pid )
		return -1;
	
	processTable[pid].sleepingPid = runningProcess->pid;
	runningProcess->state = WAITING_CHILD;
	forceMultitasker();
	*status = runningProcess->waitedStatus;
	
	return pid;
}

void waitTty(tty_t tty) {
	ttyRestPlace[tty] = runningProcess->pid;
	runningProcess->state = BLOCKED;
	forceMultitasker();
	
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

static void alertWaitingProcesses(pid_t pid, int status) {
	int i;
	process_t *process;
	
	if(pid == -1)
		return;
	
	process = &processTable[processTable[pid].ppid];
	
	if ( process->state == WAITING_CHILD )
		wakeUpProcess(process, status);

	for ( i = 0; i < MAX_PROCESS; ++i )
		if ( (processTable[i].state == WAITING_PID) && (processTable[i].waitingPid == pid) )
			wakeUpProcess(&processTable[i], status);
	
	if ( process->sleepingPid != -1 ) {
		for ( i = 0; i < MAX_TTY; ++i )
			if ( ttyRestPlace[i] == process->sleepingPid ) {
				ttyRestPlace[i] = -1;
			}
		wakeUpProcess(&processTable[process->sleepingPid], status);
	}

	return;
}

void terminate(pid_t pid, int status) {
	int i;
	pid_t ppid;
	if( pid == -1)
		return;

	ppid = processTable[pid].ppid;
	for( i = 0; i < MAX_CHILDS; ++i)
		if(processTable[ppid].childs[i] == pid)
			processTable[ppid].childs[i] = -1;
	
	processTable[ppid].childsQty--;
	/*setTtyFocusProcess(processTable[pid].tty, (processTable[pid].ppid > 0) ? processTable[pid].ppid: 0);*/
	processTable[pid].state = TERMINATED;
	processTable[pid].atomicity = UNATOMIC;
	alertWaitingProcesses(pid, status);
}