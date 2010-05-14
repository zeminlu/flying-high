#include "process.h"

extern process_t processTable[MAX_PROCESS];

extern process_t *runningProcess;

extern process_t *initProcess;

void * memmap(int isKernel){
	return (void *)int80(_SYS_MEMMAP, (void *)isKernel, NULL, NULL);
}

pid_t createProcess(char *name, pfunc_t main, void *args, int level) {
	return (pid_t)int80ext(_SYS_CREATE, name, (void*)main, args, (void *)level, NULL);
}

void exit(int status) {
	int80(_SYS_EXIT, (void *)status, NULL, NULL);
	return;
}

tty_t setTty(pid_t pid, tty_t tty) {
	return (tty_t)int80(_SYS_SET_TTY, (void *)pid, (void *)tty, NULL);
}

tty_t getTty(pid_t pid) {
	return (tty_t)int80(_SYS_GET_TTY, (void *)pid, NULL, NULL);
}

tty_t setTTYFocusedProcess(pid_t pid, tty_t tty) {
	return (tty_t)int80(_SYS_SET_TTY_FP, (void *)pid, (void *)tty, NULL);
}

tty_t getTTYFocusedProcess(tty_t tty) {
	return (tty_t)int80(_SYS_GET_TTY_FP, (void *)tty, NULL, NULL);
}

pid_t getpid(void) {
	return (pid_t)int80(_SYS_GET_PID, NULL, NULL, NULL);
}

pid_t getppid() {
	return (pid_t)int80(_SYS_GET_PPID, NULL, NULL, NULL);
}

int wait(int *status) {
	return (int)int80(_SYS_WAIT, (void *)status, NULL, NULL);
}

int waitpid(pid_t pid, int *status) {
	return (int)int80(_SYS_WAIT_PID, (void *)pid, (void *)status, NULL);
}

int kill(int pid) {
	return (int)int80(_SYS_KILL, (void *)pid, NULL, NULL);
}

int setTTYMode(pid_t pid, int mode){
	return (int)int80(_SYS_SET_TTY_MODE, (void *)pid, (void *)mode, NULL);
}

int getTTYMode(pid_t pid){
	return (int)int80(_SYS_GET_TTY_MODE, (void *)pid, NULL, NULL);
}

int setLevel(int level){
	return (int)int80(_SYS_SET_LEVEL, (void *)level, NULL, NULL);
}

int getLevel(){
	return (int)int80(_SYS_GET_LEVEL, NULL, NULL, NULL);
}

int getPriority(){
	return (int)int80(_SYS_GET_PRIO, NULL, NULL, NULL);
}

int setPriority(pid_t pid, int prio){
	return (int)int80(_SYS_SET_PRIO, (void *)pid, (void *)prio, NULL);
}

unsigned time(void){
	return (unsigned)int80(_SYS_TIME, NULL, NULL, NULL);
}

unsigned upTime(){
	return (unsigned)int80(_SYS_UPTIME, NULL, NULL, NULL);
}

int setProcessAtomicity(pid_t pid, int atomic) {
	return (int)int80(_SYS_SET_ATOM, (void *)pid, (void *)atomic, NULL);
}
