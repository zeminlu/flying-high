/*
 *	memmanager.c
 *
 *		The memory model will be KERNEL_SPACE_SIZE MBytes
 *		for kernel use and then MALLOC_SPACE_SIZE MBytes
 *		for dynamically allocated memory.
 *
 *		|-------------------------------| 0x00000000
 *		|								|
 *		|		KERNEL MEMORY			|
 *		|								|
 *		|-------------------------------| 0xKERNEL_SPACE_SIZE
 *		|								|
 *		|								|
 *		|								|
 *		|		MALLOC MEMORY			|
 *		|								|
 *		|								|
 *		|-------------------------------| 0xMALLOC_SPACE_SIZE
 *						.
 *						.
 *						.
 *		|								|
 *		|...............................| 0xFFFFFFFF
 *
 *		Author: Guido Marucci Blas
 */
#include "stdio.h"
#include "memmanager.h"
#include "string.h"
#include "debug.h"

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

/* Mememory Debug Printing Macros */
#define memoryDebug(msg)			str_debug(MEM_DEBUG, __FUNCTION__, msg, TRUE)
#define printDebugBlockStart(title) do	\
									{	\
										str_debug(MEM_DEBUG, NULL, "--------------------------------------------------------------------------------", FALSE);\
										str_debug(MEM_DEBUG, __FUNCTION__, title, TRUE);\
									}while(0)
#define printDebugBlockEnd() 		str_debug(MEM_DEBUG, NULL, "--------------------------------------------------------------------------------", FALSE)
#define printMemoryStatus() 		do 	\
									{	\
										str_debug(MEM_DEBUG, __FUNCTION__, "- Malloc Memory Status -", TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Malloc Space Start Address: ", FALSE);\
										hexa_debug(MEM_DEBUG, NULL, MALLOC_SPACE, TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Malloc Size: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, MALLOC_SPACE_SIZE, FALSE);\
										str_debug(MEM_DEBUG, NULL, " bytes", TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Free Memory: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, MALLOC_SPACE_SIZE - usedMemory.usedPages * PAGE_SIZE, FALSE);\
										str_debug(MEM_DEBUG, NULL, " bytes", TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Used Memory: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, usedMemory.usedPages * PAGE_SIZE, FALSE);\
										str_debug(MEM_DEBUG, NULL, " bytes", TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Malloc Tables Quantity: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, MALLOC_TABLES_QTY, TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "First Malloc Table: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, FIRST_MALLOC_TABLE, TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Last Malloc Table: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, FIRST_MALLOC_TABLE + MALLOC_TABLES_QTY, TRUE);\
										str_debug(MEM_DEBUG, NULL, "", TRUE);\
									} while(0)
#define printNewPageInfo(tableNumber,pageEntry,address,linealAddress) do {\
										str_debug(MEM_DEBUG, __FUNCTION__, "- New Allocated Page Information -", TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Table Number: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, (tableNumber), TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Page Entry: ", FALSE);\
										int_debug(MEM_DEBUG, NULL, (pageEntry), TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Physical Address: ", FALSE);\
										hexa_debug(MEM_DEBUG, NULL, (address), TRUE);\
										str_debug(MEM_DEBUG, __FUNCTION__, "Logical Address: ", FALSE);\
										hexa_debug(MEM_DEBUG, NULL, (int)(linealAddress), TRUE);\
									}while(0)
#define memoryError(msg) 			do 	\
									{ 	\
										str_debug(MEM_DEBUG, __FUNCTION__, "ERROR - ", FALSE);\
										str_debug(MEM_DEBUG, NULL, msg, TRUE);\
										str_debug(MEM_DEBUG, NULL, "--------------------------------------------------------------------------------", FALSE);\
									}while(0)

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
	pageTable_t pageDirectory;
} directory_t;

typedef struct frameLocation {
	unsigned int table;
	unsigned int pageEntry;
} frameLocation_t;

typedef struct usedMemBitmap {
	int usedPages;
	unsigned int bitmap[BITMAP_SIZE];
} usedMemBitmap_t;

/* The System's page directory */
static directory_t *kernelDir = (directory_t *)((char *)(KERNEL_SPACE_SIZE) - sizeof(directory_t));

/* Kernel Heap Definition */
static void *kernelHeapPages[KERNEL_HEAP_PAGES_QTY];

/*
 *	Bitmap Implementation
 */

static usedMemBitmap_t usedMemory;

/*
 *	Checks if a memory address if free.
 *	Returns TRUE or FALSE. FALSE may be
 *	returned because the page is being used
 *	or because the memory address is bigger
 *	than the MAX_MAPPED_ADDRESS.
 */
static int isFrameFree(unsigned int address) {
	int bitmap;

	if ( address > MAX_MAPPED_ADDRESS  )
		return FALSE;

	address -= MALLOC_SPACE;
	bitmap = usedMemory.bitmap[getBitmapIndex(address)];
	return bitmap & getBitmapOffset(address);
}

static void freeFrame(unsigned int address) {
	int index;

	if ( address > MAX_MAPPED_ADDRESS )
		return;

	address -= MALLOC_SPACE;
	index = getBitmapIndex(address);
	usedMemory.bitmap[index] ^= getBitmapOffset(address);
	usedMemory.usedPages--;
	
	return;
}

static void setFrameAsUsed(unsigned int address) {
	int index;

	if ( address > MAX_MAPPED_ADDRESS )
		return;

	address -= MALLOC_SPACE;
	index = getBitmapIndex(address);
	usedMemory.bitmap[index] |= getBitmapOffset(address);
	usedMemory.usedPages++;

	hexa_debug(MEM_DEBUG, __FUNCTION__, getBitmapOffset(address), TRUE);
	pause_debug(MEM_DEBUG);

	return;
}

static int getFirstFreeFrame(unsigned int *address) {
	unsigned int i;
	unsigned int mask;
	int j;

	if ( usedMemory.usedPages == MALLOC_TABLES_QTY * PAGE_ENTRIES_PER_TABLE )
		return FALSE;

	str_debug(MEM_DEBUG, __FUNCTION__, "- Bitmap Info -", TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Bitmap Size: ", FALSE);
	int_debug(MEM_DEBUG, NULL, BITMAP_SIZE, TRUE);
	for ( i = 0; i < BITMAP_SIZE; ++i ) {
		if ( usedMemory.bitmap[i] != 0xFFFFFFFF ) {
			mask = 0x00000001;
			j = 0;
			while ( (usedMemory.bitmap[i] & mask) == mask ) {
				mask = mask << 1;
				++j;
			}
			str_debug(MEM_DEBUG, __FUNCTION__, "Bitmap Number: ", FALSE);
			int_debug(MEM_DEBUG, NULL, i, TRUE);
			str_debug(MEM_DEBUG, __FUNCTION__, "Bit Number: ", FALSE);
			int_debug(MEM_DEBUG, NULL, j, TRUE);
			*address = (i * PAGES_PER_INT + j) * PAGE_SIZE  + MALLOC_SPACE;
			break;
		}
	}
	str_debug(MEM_DEBUG, NULL, "", TRUE);
	if ( i == BITMAP_SIZE )
		return FALSE;

	return TRUE;
}

/*
 *  Sets the page entry attributes
 *
 *	If kernelMode is TRUE, the page
 *	could only be accessed by kernel code.
 *	If not the page could also be accessed
 *	by user code.
 *
 *  If writeable is FALSE the page will be set
 *  as read-only.
 */
static void setPageEntry(page_t *page, int kernelMode, int writeable, int present) {
	page->present = (present)?1:0;
	page->writeable = (writeable)?1:0;
	page->user = (kernelMode)?0:1;
	return;
}

/*
 *	Allocates a new page frame
 *	into a page entry for the kernel
 *	with kernel access mode and write
 *	access.
 */
static int kpageAlloc(page_t *page) {
	static unsigned memory = KERNEL_SPACE;

	if ( memory == KERNEL_SPACE_SIZE )
		return FALSE;

	setPageEntry(page, TRUE, TRUE, TRUE);
	page->address = memory >> 12; /* The address field has 20 bits for the base address */
	memory += PAGE_SIZE;

	return TRUE;
}

/*
 *	Initializes a page table with all it's
 *	pages in kernel mode and with write
 *	access.
 */
static int initKernelPageTable(pageTable_t table) { 
	unsigned int i;

	for ( i = 0; i < PAGE_ENTRIES_PER_TABLE; ++i )
		if ( !kpageAlloc(&table[i]) )
			return FALSE;
	return TRUE;
}

static void initMallocPageTable(pageTable_t table) {
	unsigned int i;

	for ( i = 0; i < PAGE_ENTRIES_PER_TABLE; ++i ) {
		table[i].address = 0x0;
		setPageEntry(&table[i], TRUE, FALSE, FALSE);
	}

	return;
}

static void *setLinealAddress(int table, int pageEntry) {
	unsigned int linealAddress = 0;

	linealAddress = table << 22;
	linealAddress |= pageEntry << 12; 

	return (void *)linealAddress; 
}

void getFrameLocation(unsigned int linealAddress, frameLocation_t *location) {
	location->table = (linealAddress & 0xFFC00000) >> 22;
	location->pageEntry = (linealAddress & 0x0003FF000) >> 12;
	return;
}

static unsigned int getPhysicalAddress(frameLocation_t location) {
	return (kernelDir->tables[location.table][location.pageEntry].address) << 12;
}

static unsigned int getBaseFrameAddress(unsigned int address) {
	return address - (address % PAGE_SIZE);
}

int initPageDirectory(void) {
	unsigned int i;
	unsigned int cr0;

	memset(kernelDir, 0, sizeof(directory_t));
	for ( i = FIRST_KERNEL_TABLE; i < KERNEL_TABLES_QTY; ++i ) {
		if ( !initKernelPageTable(kernelDir->tables[i]) )
			return FALSE; 
		setPageEntry(&(kernelDir->pageDirectory[i]), TRUE, TRUE, TRUE);
		kernelDir->pageDirectory[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}
	for ( ; i < PAGE_TABLES_QTY; ++i ) {
		initMallocPageTable(kernelDir->tables[i]);
		setPageEntry(&(kernelDir->pageDirectory[i]), TRUE, TRUE, TRUE);
		kernelDir->pageDirectory[i].address = ((unsigned int)(&(kernelDir->tables[i]))) >> 12;
	}

	asm volatile("mov %0, %%cr3":: "r"(&(kernelDir->pageDirectory)));
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
			
	for ( i = 0; i < KERNEL_HEAP_PAGES_QTY; ++i )
		kernelHeapPages[i] = pageAlloc(TRUE);

	return TRUE;
}

void *pageAlloc(int kernel) {
	unsigned int address;
	unsigned int i, j;
	void *linealAddress;
	page_t *table;/* With pageTable_t table it doesn't compile. DON'T ASK!!!  */
	page_t *page;
	
	printDebugBlockStart("New Page Allocation Request.\n");
	printMemoryStatus();

	if ( !getFirstFreeFrame(&address) ) {
		memoryError("No Free page frame could be found.");
		return NULL;
	}
		
	hexa_debug(MEM_DEBUG, __FUNCTION__, address, TRUE);
	setFrameAsUsed(address);
	for ( i = FIRST_MALLOC_TABLE; i < FIRST_MALLOC_TABLE + MALLOC_TABLES_QTY; ++i ) {
		table = kernelDir->tables[i];
		for ( j = 0; j < PAGE_ENTRIES_PER_TABLE; ++j ) { 
			page = &table[j];
			if ( page->address == 0x000000000 ) {
				page->address = address >> 12;
				setPageEntry(page, kernel, TRUE, FALSE);
				linealAddress = setLinealAddress(i,j);
				
				printNewPageInfo(i, j, address, (unsigned int)linealAddress);
				printDebugBlockEnd();

				return linealAddress;
			}
		}
	}
	
	memoryError("No Free page entry could be found.");
	freeFrame(address);
	return NULL;
}

void *pageDistinctAlloc(int kernel, void *address) {
	unsigned int phyAddress, phyAllocated;
	frameLocation_t location1, location2;
	void *temp, *page;

	getFrameLocation((unsigned int)address, &location1);
	phyAddress = getPhysicalAddress(location1);
	if (location1.table >= PAGE_TABLES_QTY || location1.table < KERNEL_TABLES_QTY)
		return NULL;
	page = pageAlloc(kernel);
	getFrameLocation((unsigned int)page, &location2);
	phyAllocated = getPhysicalAddress(location2);
	if ( phyAllocated == phyAddress + PAGE_SIZE ) {
		temp = page;
		page = pageAlloc(kernel);
		freePage(temp);
	}

	return page;
}

/*
 *	Strange things happen when running
 *	MonoOS on Bochs. When pages are set
 *	free sometimes in Bochs their are
 *	kept accesible even when it's presence
 *	bit is set to 0.
 *
 * 	The following code works different on
 * 	Bochs and  VBox
 *
 *	page = pageAlloc(FALSE);
 *	setPagePresence(page, TRUE);
 *	printPageInfo(page);
 *	
 *	strcpy(page, "Hola Putos 1\n");
 *	puts((unsigned char *)page);
 *	freePage(page);
 *	printPageInfo(page);
 *	puts((unsigned char *)"\nMETIENDO EN PAGE1 - DEBERIA DAR FAULT\n");
 *	*((char *)page) = 'c';
 *	puts((unsigned char *)"\nLEYENDO EN PAGE - DEBERIA DAR FAULT\n");
 *	puts((unsigned char *)page);
 *
 */
void freePage(void *pageAddr) {
	frameLocation_t location;

	printDebugBlockStart("Free Page Request.\n");
	if ( pageAddr == NULL ) {
		memoryDebug("ERROR - A NULL address was given.");
		return;
	}
	
	getFrameLocation((unsigned int)pageAddr, &location);
	if (location.table >= PAGE_TABLES_QTY || location.table < KERNEL_TABLES_QTY) {
		memoryDebug("ERROR - Trying to free an invalid address.");
		return;
	}

	kernelDir->tables[location.table][location.pageEntry].address = 0x0;
	kernelDir->tables[location.table][location.pageEntry].present = 0;
	kernelDir->tables[location.table][location.pageEntry].writeable = 0;
	freeFrame(kernelDir->tables[location.table][location.pageEntry].address);
	
	str_debug(MEM_DEBUG, __FUNCTION__, "Address ", FALSE);
	hexa_debug(MEM_DEBUG, NULL, (int)pageAddr, FALSE);
	str_debug(MEM_DEBUG, NULL, " has been freed.", TRUE);
	printDebugBlockEnd();

	return;
}

void setPagePresence(void *address, int presence) {
	frameLocation_t location;

	if ( address == NULL )
		return;
	/*
	str_debug(MEM_DEBUG, __FUNCTION__, "Getting page frame location from ", FALSE);
	hexa_debug(MEM_DEBUG, NULL, (int)address, TRUE);
	*/
	getFrameLocation((unsigned int)address, &location);

	/*
	memoryDebug("");
	memoryDebug("- Page Info -");
	str_debug(MEM_DEBUG, __FUNCTION__, "Page Table: ", FALSE);
	int_debug(MEM_DEBUG, NULL, location.table, TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Page Entry: ", FALSE);
	int_debug(MEM_DEBUG, NULL, location.pageEntry, TRUE);
	*/

	if (location.table >= PAGE_TABLES_QTY || location.table < KERNEL_TABLES_QTY) {
		//memoryDebug("The presence bit could not be set.");
		return;
	}
	
	/*
	str_debug(MEM_DEBUG, __FUNCTION__, "Setting presence bit of address ", FALSE);
	hexa_debug(MEM_DEBUG, NULL, (int)address, FALSE);
	str_debug(MEM_DEBUG, NULL, (presence)?" as present":" as not present", TRUE);
	str_debug(MEM_DEBUG, NULL, "", TRUE);
	*/
	kernelDir->tables[location.table][location.pageEntry].present = (presence)?1:0;
	
	return;
}

void setPageMode(void *address, int kernel) {
	frameLocation_t location;

	if ( address == NULL )
		return;
	getFrameLocation((unsigned int)address, &location);
	if ( location.table >= PAGE_TABLES_QTY || location.table < KERNEL_TABLES_QTY )
		return;

	kernelDir->tables[location.table][location.pageEntry].user = (kernel)?0:1;

	return;
}

void setPageAccess(void *address, int writeable) {
	frameLocation_t location;

	if ( address == NULL )
		return;
	getFrameLocation((unsigned int)address, &location);
	if ( location.table >= PAGE_TABLES_QTY || location.table < KERNEL_TABLES_QTY )
		return;

	kernelDir->tables[location.table][location.pageEntry].writeable = (writeable)?0:1;

	return;
}

/* Memory Info Getters */
int getTotalMemorySize(void) {
	return TOTAL_MEMORY_SIZE;
}

int getKernelMemorySize(void) {
	return KERNEL_SPACE_SIZE;
}

int getMallocMemorySize(void) {
	return MALLOC_SPACE_SIZE;
}

int getFreeMallocMemorySize(void) {
	return MALLOC_SPACE_SIZE - (usedMemory.usedPages * PAGE_SIZE); 
}

int getUsedMallocMemorySize(void) {
	return (usedMemory.usedPages * PAGE_SIZE); 
}

int getPageSize(void) {
	return PAGE_SIZE;
}

void *getKernelHeap(void) {
	return kernelHeapPages[0];
}

unsigned getKernelHeapSize(void) {
	return KERNEL_HEAP_PAGES_QTY * PAGE_SIZE;
}

void setKernelHeapPresence(int present) {
	int i;

	for ( i = 0; i < KERNEL_HEAP_PAGES_QTY; ++i )
		if ( kernelHeapPages[i] != NULL )
			setPagePresence(kernelHeapPages[i], present);

	return;
}

/* Debug Functions */
void printPageInfo(void *address) {
	frameLocation_t location;

	getFrameLocation((unsigned int)address, &location);
	str_debug(MEM_DEBUG, __FUNCTION__, "- Page Info -", TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Lineal Address: ", FALSE);
	hexa_debug(MEM_DEBUG, NULL, (int)address, TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Physical Address: ", FALSE);
	hexa_debug(MEM_DEBUG, NULL, (kernelDir->tables[location.table][location.pageEntry].address) << 12, TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Table: ", FALSE);
	int_debug(MEM_DEBUG, NULL, location.table, TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Page Entry: ", FALSE);
	int_debug(MEM_DEBUG, NULL, location.pageEntry, TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Is Page Present: ", FALSE);
	str_debug(MEM_DEBUG, NULL, (kernelDir->tables[location.table][location.pageEntry].present)?"true":"false", TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Is Page Writable: ", FALSE);
	str_debug(MEM_DEBUG, NULL, (kernelDir->tables[location.table][location.pageEntry].writeable)?"true":"false", TRUE);
	str_debug(MEM_DEBUG, __FUNCTION__, "Privilage: ", FALSE);
	str_debug(MEM_DEBUG, NULL, (kernelDir->tables[location.table][location.pageEntry].user)?"user":"kernel", TRUE);
	str_debug(MEM_DEBUG, NULL, "", TRUE);

	return;
}

void printMemoryInfo(void) {
	printMemoryStatus();
	return;
}
