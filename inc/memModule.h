#ifndef _MEMMODULE_H_
#define _MEMMODULE_H_

#include "string.h"
#include "stdio.h"
#include "types.h"

void loadCR3();

int initPaging();

void setKernelHeapPresence(int state);

void initPage(page_t *page, int isKernel);

int allocKPage(page_t *kernelPage);

int initKernelTable(pageTable_t kernelTable);

int allocMPage(page_t *mallocPage);

int initMallocTable(pageTable_t mallocTable, int dirIndex);

frame_t * getFrame();

int freeFrame(frame_t *frame);

void setFramePresence(frame_t *frame, int state);

#endif
