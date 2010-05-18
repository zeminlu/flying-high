/**
 *	\file sysProcess.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "sysProcess.h"

unsigned int tickCount = 0;

process_t processTable[MAX_PROCESS];

process_t *runningProcess = NULL;

process_t *initProcess = NULL;

process_t *nextProcess = NULL;

static int mtActivated = FALSE;

static pid_t ttyRestPlace[MAX_TTY];

FILE fileSystems[MAX_PROCESS][MAX_FILES];

char fileBuffers[MAX_PROCESS][MAX_FILES][SCREEN_SIZE];

int qtyProccessTable = 0;

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
					processTable[i].name[0]=  '\0';
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
		processTable[i].childsQty = 0;
		processTable[i].rpgOld = 0;
		for ( j = 0; j < MAX_FILES; ++j ){
			processTable[i].files[j] = &(fileSystems[i][j]);
			processTable[i].files[j]->ptr = processTable[i].files[j]->buffer = fileBuffers[i][j];
			processTable[i].files[j]->fd = j;
			processTable[i].files[j]->flag = (_READ | _WRITE);
			processTable[i].files[j]->bufferSize = SCREEN_SIZE;
		}
	}
	
	if ( (pid = createProcess("init", init, 0, BACKGROUND)) == -1 ) {
		return FALSE;
	}
	
	for ( i = 0; i < MAX_TTY; ++i )
		ttyRestPlace[i] = -1;
	initProcess = &processTable[pid];
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
	nextProcess = (runningProcess == NULL) ? nextProcess : getNextTask();
	if ( nextProcess != runningProcess) {
		if (nextProcess == initProcess && runningProcess != NULL && runningProcess->state == RUNNING){
			nextProcess = runningProcess;
		}
		else{
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
		}
	}else {
	}
	freeTerminatedProcesses();
	runningProcess = nextProcess;
	++runningProcess->tickCounter;
	if (runningProcess != initProcess && runningProcess->level == FOREGROUND ) 
		sysSetTTYFocusedProcess(runningProcess->pid, runningProcess->tty);
	
	return;
}

static void wakeUpProcess(process_t *process, int status) {
	process->waitedStatus = status;
	process->state = READY;

	return;
}

void sysSelfBlock(){
	runningProcess->state = BLOCKED;
	forceMultitasker();
	
	return;
}

int sysUnblock(pid_t pid){
	if (processTable[pid].state != BLOCKED){
		return -1;
	}
	processTable[pid].state = READY;
	
	return 0;
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
	if (processTable[pid].level == FOREGROUND && processTable[pid].childsQty == 0){
		sysSetTTYFocusedProcess((ppid > 0) ? ppid : 0, processTable[pid].tty);
	}
	alertWaitingProcesses(pid, status);
	processTable[pid].state = TERMINATED;
	processTable[pid].atomicity = UNATOMIC;
}

void refreshProcessesCPUs(){
	int i;
	double total = 0;
	
	for (i = 0 ; i < MAX_PROCESS ; ++i){
		if (processTable[i].pid != -1){
			total += processTable[i].tickCounter;
		}
	}
	for (i = 0 ; i < MAX_PROCESS ; ++i){
		if (processTable[i].pid != -1){
			processTable[i].cpuPercent = processTable[i].tickCounter / total * 100;
		}
	}
	return;
}

void clearProcessesTicks(){
	int i;
	
	for (i = 0 ; i < MAX_PROCESS ; ++i){
		if (processTable[i].pid != -1){
			processTable[i].tickCounter = 0;
		}
	}
}

process_t *getProcessTable(void) {
	return processTable;
}

char *getProcessName(pid_t pid) {
	if ( pid > MAX_PROCESS || pid < 0 )
		return NULL;
	return processTable[pid].name;
}