/*
 *	process.c
 *
 *		Implements the interface process.h by calling interrupt 80
 *
 */
#include "stdio.h"
#include "callint80.h"
#include "process.h"
#include "unistd.h"

pid_t createProcess(char *name, pfunc_t main, int args, int level) {
	return (pid_t)callInt802(ID_FORK, name, (void*)main, (void *)args, (void *)level, NULL);
}

void exit(int status) {
	callInt80(ID_EXIT, (void *)status, NULL, NULL);
	return;
}

tty_t setTty(pid_t pid, tty_t tty) {
	return (tty_t)callInt80(ID_SET_TTY, (void *)pid, (void *)tty, NULL);
}

tty_t getTty(pid_t pid) {
	return (tty_t)callInt80(ID_GET_TTY, (void *)pid, NULL, NULL);
}

pid_t getpid(void) {
	return (pid_t)callInt80(ID_GET_PID, NULL, NULL, NULL);
}

int getpriority(void) {
	return (int)callInt80(ID_GET_PRIO, NULL, NULL, NULL);
}

int setpriority(pid_t pid, int prio) {
	return (int)callInt80(ID_SET_PRIO, (void *)pid, (void *)prio, NULL);
}

int getlevel(void) {
	return (int)callInt80(ID_GET_LEVEL, NULL, NULL, NULL);
}

int setlevel(int level) {
	return (int)callInt80(ID_SET_LEVEL, (void *)level, NULL, NULL);
}

int dup(int fd) {
	return (int)callInt80(ID_DUP, (void *)fd, NULL, NULL);
}

int dup2(int fd1, int fd2) {
	return (int)callInt80(ID_DUP_2, (void *)fd1, (void *)fd2, NULL);
}

int waitpid(pid_t pid, int *status, int opt) {
	return (int)callInt80(ID_WAIT_PID, (void *)pid, (void *)status, (void *)opt);
}

int kill(int fd1, int fd2) {
	return (int)callInt80(ID_KILL, (void *)fd1, (void *)fd2, NULL);
}
