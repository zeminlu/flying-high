#include "sysProcess.h"

process_t processTable[MAX_PROCESS];

process_t *runningProcess = NULL;

process_t *idleProcess = NULL;

process_t *nextProcess = NULL;

static pid_t nextPID = 1;

int qtyProccessTable = 0;

