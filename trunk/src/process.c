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

pid_t getpid(void) {
	return (pid_t)int80(_SYS_GET_PID, NULL, NULL, NULL);
}

pid_t getppid() {
	return (pid_t)int80(_SYS_GET_PPID, NULL, NULL, NULL);
}

int waitpid(pid_t pid, int *status, int opt) {
	return (int)int80(_SYS_WAIT_PID, (void *)pid, (void *)status, (void *)opt);
}

int kill(int fd1, int fd2) {
	return (int)int80(_SYS_KILL, (void *)fd1, (void *)fd2, NULL);
}