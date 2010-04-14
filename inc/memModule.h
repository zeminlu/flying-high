#ifndef _MEMMODULE_H_
#define _MEMMODULE_H_

#include "string.h"

/*	Units Definitions */
#define KB							* (1 << 10)
#define MB							* (1 << 20)

/* Memory Space Model */
#define KERNEL_SPACE				0x00000000
#define KERNEL_SPACE_SIZE			(8 MB)
#define MALLOC_SPACE				(KERNEL_SPACE + KERNEL_SPACE_SIZE)
#define MALLOC_SPACE_SIZE			(8 MB)
#define TOTAL_MEMORY_SIZE			(KERNEL_SPACE_SIZE + MALLOC_SPACE)

/* Page Definitions */
#define PAGE_SIZE					(4 KB)
#define PAGE_ENTRY_SIZE				sizeof(unsigned int)
#define PAGE_ENTRIES_PER_TABLE		(PAGE_SIZE / PAGE_ENTRY_SIZE)
#define KERNEL_TABLES_QTY			(KERNEL_SPACE_SIZE / (PAGE_SIZE * PAGE_ENTRIES_PER_TABLE))
#define MALLOC_TABLES_QTY			(MALLOC_SPACE_SIZE / (PAGE_SIZE * PAGE_ENTRIES_PER_TABLE))
#define PAGE_TABLES_QTY				(KERNEL_TABLES_QTY + MALLOC_TABLES_QTY)

/* Table's Index Definitions */
#define FIRST_KERNEL_TABLE			0
#define FIRST_MALLOC_TABLE			FIRST_KERNEL_TABLE + KERNEL_TABLES_QTY

/* Free Memory Bitmap Definitions */
#define PAGES_PER_INT				((int)(sizeof(unsigned int)) * 8)
#define INTS_PER_TABLE				(PAGE_ENTRIES_PER_TABLE / 32)
#define BITMAP_SIZE					(INTS_PER_TABLE * MALLOC_TABLES_QTY)
#define MAX_MAPPED_ADDRESS			(TOTAL_MEMORY_SIZE - 1)
#define getBitmapIndex(addr)		(((addr) / (unsigned)PAGE_SIZE) / (unsigned)PAGES_PER_INT)
#define getBitmapOffset(addr)		(0x00000001 << (((addr) / PAGE_SIZE) % PAGES_PER_INT))
#define OFFSET_MASK					0xFFFFF000

/* Kernel Heap Area */
#define KERNEL_HEAP_PAGES_QTY		10

typedef struct page {
	unsigned int present 		: 1;
	unsigned int writeable 		: 1;
	unsigned int user 			: 1;
	unsigned int directWrite	: 1;
	unsigned int disableCache	: 1;
	unsigned int accessed		: 1;
	unsigned int dirty			: 1;
	unsigned int reserved		: 5;
	unsigned int address		: 20;
} page_t;

typedef page_t pageTable_t[PAGE_ENTRIES_PER_TABLE];

typedef struct directory {
	pageTable_t tables[PAGE_TABLES_QTY];
	pageTable_t pageTable;
} directory_t;

typedef struct frameLocation {
	unsigned int table;
	unsigned int pageEntry;
} frameLocation_t;

typedef struct usedMemBitmap {
	int usedPages;
	unsigned int bitmap[BITMAP_SIZE];
} usedMemBitmap_t;

int initPaging();

void initPage(page_t *page, int isKernel);

int allocKPage(page_t *kernelPage);

int initKernelTable(pageTable_t kernelTable);

int allocMPage(page_t *mallocPage);

int initMallocTable(pageTable_t mallocTable);




#endif