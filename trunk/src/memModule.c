#include "../inc/memModule.h"

/* The System's page directory */
static directory_t *kernelDir = (directory_t *)((char *)(KERNEL_SPACE_SIZE) - sizeof(directory_t));

/* Kernel Heap Definition */
static void *kernelHeapPages[KERNEL_HEAP_PAGES_QTY];

/*
 *	Bitmap Implementation
 */

static usedMemBitmap_t usedMemory;

int initPaging(){
	unsigned int i;
	unsigned int cr0;

	memset(kernelDir, 0, sizeof(directory_t));
	for (i = FIRST_KERNEL_TABLE ; i < KERNEL_TABLES_QTY ; ++i){
		initKernelTable(kernelDir->tables[i]);
		initPage(&(kernelDir->pageTable[i]), TRUE);
		kernelDir->pageTable[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}
	for (i = FIRST_MALLOC_TABLE ; i < MALLOC_TABLES_QTY ; ++i){
		initMallocTable(kernelDir->tables[i]);
		initPage(&(kernelDir->pageTable[i]), FALSE);
		kernelDir->pageTable[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}
	
	asm volatile("mov %0, %%cr3":: "r"(&(kernelDir->pageTable)));
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
	
	return 0;
}

void initPage(page_t *page, int isKernel){
	page->present = TRUE;
	page->writeable = isKernel;
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

int initMallocTable(pageTable_t mallocTable){
	int i;
	
	for (i = 0 ; i < PAGE_ENTRIES_PER_TABLE ; ++i){
		allocMPage(&(mallocTable[i]));
	}

	return 0;
}