/**
 *	\file memModule.h
 *
 *		\brief The memory management / paging module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _MEMMODULE_H_
#define _MEMMODULE_H_

#include "string.h"
#include "stdio.h"
#include "types.h"

/**
 * \fn void loadCR3()
 *
 * 		\brief Loads the CR3 into the processor register.
 *
 */
void loadCR3();

/**
 * \fn int initPaging()
 *
 * 		\brief Initializes the pagination and turns it on.
 *
 * 		\return 0 on success, -1 on error.
 *
 */
int initPaging();

/**
 * \fn void setKernelHeapPresence(int state)
 *
 * 		\brief Sets the presence bit of the kernel heap pages according to the given parameter.
 *
 * 		\param state True or False.
 * 		
 */
void setKernelHeapPresence(int state);

/**
 * \fn void initPage(page_t *page, int isKernel)
 *
 * 		\brief Initializes the given page with according permissions.
 *
 * 		\param page A pointer to the page.
 * 		\param isKernel Permissions, True or False.
 * 		
 */
void initPage(page_t *page, int isKernel);

/**
 * \fn int allocKPage(page_t *kernelPage)
 *
 * 		\brief Allocs a Page for the kernel.
 *
 * 		\param kernelPage A pointer to the page.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 */
int allocKPage(page_t *kernelPage);

/**
 * \fn int initKernelTable(pageTable_t kernelTable)
 *
 * 		\brief Initializes a kernel page table.
 *
 * 		\param kernelTable The table to be initialized.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 */
int initKernelTable(pageTable_t kernelTable);

/**
 * \fn int allocMPage(page_t *mallocPage)
 *
 * 		\brief Allocs a Page for the malloc section.
 *
 * 		\param mallocPage A pointer to the page.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 */
int allocMPage(page_t *mallocPage);

/**
 * \fn int initMallocTable(pageTable_t mallocTable, int dirIndex)
 *
 * 		\brief Initializes a malloc page table.
 *
 * 		\param dirIndex Reserved.
 * 		\param mallocTable The table to be initialized.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 */
int initMallocTable(pageTable_t mallocTable, int dirIndex);

/**
 * \fn frame_t * getFrame()
 *
 * 		\brief Gets a free frame.
 *
 * 		\return A pointer to a frame.
 *
 */
frame_t * getFrame();

/**
 * \fn int freeFrame(frame_t *frame)
 *
 * 		\brief Frees the given frame.
 *
 * 		\param frame A pointer to the frame.
 * 		
 * 		\return 0 on success, -1 on error.
 *
 */
int freeFrame(frame_t *frame);

/**
 * \fn void setFramePresence(frame_t *frame, int state)
 *
 * 		\brief Sets the presence bit of the pages from the given frame to the given state.
 *
 * 		\param frame A pointer to the frame.
 * 		\param state True or False.
 * 		
 */
void setFramePresence(frame_t *frame, int state);

#endif
