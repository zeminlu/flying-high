/*
 *	memmanager.h
 *
 * 		This module manages the memory and provides
 * 		the functions to dynamically allocate pages.
 *
 *		Author: Guido Marucci Blas
 */

#ifndef MEMMANAGER_H_
#define MEMMANAGER_H_

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE !TRUE
#endif

#ifndef NULL
#define NULL (void *)0x0
#endif

/**
 *	@fn void initPageDirectory(void);
 *
 *		@brief This function initializes the
 *		page directory and sets the processor's
 *		paging mode on. This function must be called
 *		before any dinamically allocated memory is used.
 *		@return TRUE or FALSE
 */
int initPageDirectory(void);

/**
 *	@fn void *pageAlloc(int kernel)
 *
 *		@brief This function allocates a new page.
 *		@param kernel If TRUE the page will be set
 *		in kernel mode, if not in user mode.
 *		@return The address of the new allocated page 
 *		or NULL in case of error.
 */
void *pageAlloc(int kernel);

/**
 *	@fn void *pageDistinctAlloc(int kernel, void *basePage)
 *
 *		@brief This function allocates a new page that will
 *		not be contiguos to basePage.
 *		@param kernel If TRUE the page will be set
 *		in kernel mode, if not in user mode.
 *		@param basePage The base page address from where the
 *		new allocated page will not be contiguos. If basePage
 *		is not multiple of PAGE_SIZE then basePage will be
 *		changed to be beginning of the page.
 *		@return The address of the new allocated page 
 *		or NULL in case of error.
 *
 */
void *pageDistinctAlloc(int kernel, void *basePage);

/**
 *	@fn void freePage(void *pageAddr)
 *
 *		@brief This function frees a page that 
 *		was allocated with pageAlloc.
 *		@param pageAdd The page's address that
 *		wants to be freed.
 */
void freePage(void *pageAddr);

/**
 *	@fn void setPagePresense(void *address, int presence)
 *		
 *		@brief This function sets the page's presence
 *		bit.
 *		@param address The page's address.
 *		@param presence TRUE or FALSE.
 *
 */
void setPagePresence(void *address, int presence);

/**
 *	@fn void setPageMode(void *address, int kernel)
 *
 *		@brief This function sets the page's user
 *		bit.
 *		@param address The page's address.
 *		@param kernel TRUE if kernel mode
 *
 */
void setPageMode(void *address, int kernel);

/**
 *	@fn void setPageAccess(void *address, int writeable)
 *
 *		@brief This function sets the page's writeable
 *		bit.
 *		@param address The page's address.
 *		@param writeable
 *
 */
void setPageAccess(void *address, int writeable);

int getTotalMemorySize(void); 

int getKernelMemorySize(void);

int getMallocMemorySize(void);

int getFreeMallocMemorySize(void);

int getUsedMallocMemorySize(void);

int getPageSize(void);

void *getKernelHeap(void);

unsigned getKernelHeapSize(void);

void setKernelHeapPresence(int present); 

/**
 * 	@fn void printPageInfo(void *address)
 *
 * 		@brief This function is for debugging use only.
 * 		The DEBUG_LEVEL symbol must be set to MEM_DEBUG, if not
 * 		this function will not print.
 * 		@param address The page's address.
 *
 */
void printPageInfo(void *address);

/**
 *	@fn printMemoryInfo(void)
 *
 *		@brief This function is for debugging use only.
 * 		The DEBUG_LEVEL symbol must be set to MEM_DEBUG, if not
 * 		this function will not print.
 * 		@param address The page's address.
 *
 */
void printMemoryInfo(void);

#endif
