#include "sysProcess.h"

process_t processTable[MAX_PROCESS];

process_t *runningProcess;

process_t *initProcess;

process_t *nextProcess;

static pid_t nextPID = 1;

sysTTY ttyTable;

int qtyProccessTable = 0;

