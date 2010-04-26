#include "../inc/memModule.h"

/* The System's page directory */
static directory_t *kernelDir = (directory_t *)((char *)(KERNEL_SPACE_SIZE) - sizeof(directory_t));

/* Kernel Heap Definition */
static void *kernelHeap;
static frame_t *kernelFrame;

/* Frame table Definition*/
static framesTable_t framesTable;

/*
 *	Bitmap Implementation
 */

/*static usedMemBitmap_t usedMemory;*/

void * getKernelHeap(){
	return kernelHeap;
}

int initPaging(){
	unsigned int i;
	unsigned int cr0;

	memset(kernelDir, 0, sizeof(directory_t));
	memset(framesTable, 0, sizeof(frame_t) * TOTAL_FRAMES_QTY);
	
	for (i = FIRST_KERNEL_TABLE ; i < KERNEL_TABLES_QTY ; ++i){
		initKernelTable(kernelDir->tables[i]);
		initPage(&(kernelDir->pageTable[i]), TRUE);
		kernelDir->pageTable[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}
	for ( ; i < PAGE_TABLES_QTY ; ++i){
		initMallocTable(kernelDir->tables[i], i);
		initPage(&(kernelDir->pageTable[i]), FALSE);
		kernelDir->pageTable[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}
	
	asm volatile("mov %0, %%cr3":: "r"(&(kernelDir->pageTable)));
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
	
	kernelFrame = getFrame();
	kernelHeap = (void *)kernelFrame->address;
	
	/*puts("kernelHeap address = ");
	putx((int)kernelFrame->address);
	puts("\n");
	flushStdScreen();*/
	
	return 0;
}

void setKernelHeapPresence(int state){
	setFramePresence(kernelFrame, state);
}

void initPage(page_t *page, int isKernel){
	page->present = isKernel;
	page->writeable = !isKernel;
	page->user = isKernel;
	
	return;
}

int allocKPage(page_t *kernelPage){
	static unsigned int add = KERNEL_SPACE;
	
	initPage(kernelPage, TRUE);
	kernelPage->address = add >> 12;	
	add += PAGE_SIZE;
	
	return 0;
}

int initKernelTable(pageTable_t kernelTable){
	int i;
	
	for (i = 0 ; i < PAGE_ENTRIES_PER_TABLE ; ++i){
		allocKPage(&(kernelTable[i]));
	}
	
	return 0;
}

int allocMPage(page_t *mallocPage){
	static unsigned int add = MALLOC_SPACE;
	
	initPage(mallocPage, FALSE);
	mallocPage->address = add >> 12;
	add += PAGE_SIZE;
	
	return 0;
}

int initMallocTable(pageTable_t mallocTable, int dirIndex){
	int i, j;
	static int frameIndex = 0;
	unsigned int dir = 0, table = 0;
	
	dir = dirIndex << 22;
	for (i = 0, j = 0 ; i < PAGE_ENTRIES_PER_TABLE ; ++i){
		allocMPage(&(mallocTable[i]));
		if (j == PAGES_PER_FRAME){
			table = i << 12;
			framesTable[frameIndex].address |= dir;
			framesTable[frameIndex].address |= table; 
			j = 0;
			++frameIndex;
		}else{
			++j;
		}
	}

	return 0;
}

void setFramePresence(frame_t *frame, int state){
	int i;
	unsigned int dirIndex = 0, tableIndex = 0;
	
	dirIndex = frame->address >> 22;
	for (i = 0 ; i < PAGES_PER_FRAME ; ++i){
		tableIndex = (frame->address + i * PAGE_SIZE) >> 12;
		((kernelDir->tables)[dirIndex][tableIndex]).present = state;
	}
	return;
}

frame_t * getFrame(){
	int i = 0;
	
	for (i = 0 ; i < TOTAL_FRAMES_QTY ; ++i){
		if (!((framesTable[i]).assigned)){
			framesTable[i].assigned = TRUE;
			return &(framesTable[i]); 
		}
	}
	return NULL;
}

int freeFrame(frame_t *frame){
	if (!frame->assigned){
		return -1;
	}
	else{
		frame->assigned = FALSE;
		return 0;
	}
}