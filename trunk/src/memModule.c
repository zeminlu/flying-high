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

void loadCR3(){
	asm volatile("mov %0, %%cr3":: "r"(&(kernelDir->pageDirectory)));
	
	return;
}

int initPaging(){
	unsigned int i, cr0;

	memset(kernelDir, 0, sizeof(directory_t));
	memset(framesTable, 0, sizeof(frame_t) * TOTAL_FRAMES_QTY);
	
	for (i = FIRST_KERNEL_TABLE ; i < KERNEL_TABLES_QTY ; ++i){
		initKernelTable(kernelDir->tables[i]);
		initPage(&(kernelDir->pageDirectory[i]), TRUE);
		kernelDir->pageDirectory[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}
	for ( ; i < PAGE_TABLES_QTY ; ++i){
		initMallocTable(kernelDir->tables[i], i);
		initPage(&(kernelDir->pageDirectory[i]), FALSE);
		kernelDir->pageDirectory[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}
	
	loadCR3();
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
		
	kernelFrame = getFrame();
	kernelHeap = (void *)kernelFrame->address;
	setKernelHeapPresence(FALSE);
	
	return 0;
}

void setKernelHeapPresence(int state){
	setFramePresence(kernelFrame, state);
}

void initPage(page_t *page, int isKernel){
	page->present = TRUE;
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
		if (j == 0){
			table = i << 12;
			framesTable[frameIndex].address |= dir;
			framesTable[frameIndex].address |= table;
			++frameIndex;
			++j;
		}else{
			j = ((j + 1) == PAGES_PER_FRAME) ? 0 : j + 1;
		}
	}

	return 0;
}

void setFramePresence(frame_t *frame, int state){
	int i;
	unsigned int dirIndex = 0, tableIndex = 0;
	
	dirIndex = frame->address >> 22;
	tableIndex = (frame->address & 0x003FFFFF) >> 12;
	for (i = 0 ; i < PAGES_PER_FRAME ; ++i){
		((kernelDir->tables)[dirIndex][tableIndex + i]).present = state;
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