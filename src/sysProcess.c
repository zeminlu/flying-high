#include "sysProcess.h"

process_t processTable[MAX_PROCESS];

process_t *runningProcess = NULL;

process_t *initProcess = NULL;

process_t *nextProcess = NULL;

pid_t currentPID = 0;

static pid_t nextPID = 1;

sysTTY ttyTable;

int qtyProccessTable = 0;

