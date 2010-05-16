/**
 *	\file memModule.h
 *
 *		\brief Brief.
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
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void loadCR3();

/**
 * \fn int initPaging()
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int initPaging();

/**
 * \fn void setKernelHeapPresence(int state)
 *
 * 		\brief Brief.
 *
 * 		\param state ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void setKernelHeapPresence(int state);

/**
 * \fn void initPage(page_t *page, int isKernel)
 *
 * 		\brief Brief.
 *
 * 		\param page ParamBrief.
 * 		\param isKernel ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void initPage(page_t *page, int isKernel);

/**
 * \fn int allocKPage(page_t *kernelPage)
 *
 * 		\brief Brief.
 *
 * 		\param kernelPage ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int allocKPage(page_t *kernelPage);

/**
 * \fn int initKernelTable(pageTable_t kernelTable)
 *
 * 		\brief Brief.
 *
 * 		\param kernelTable ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int initKernelTable(pageTable_t kernelTable);

/**
 * \fn int allocMPage(page_t *mallocPage)
 *
 * 		\brief Brief.
 *
 * 		\param mallocPage ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int allocMPage(page_t *mallocPage);

/**
 * \fn int initMallocTable(pageTable_t mallocTable, int dirIndex)
 *
 * 		\brief Brief.
 *
 * 		\param dirIndex ParamBrief.
 * 		\param mallocTable ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int initMallocTable(pageTable_t mallocTable, int dirIndex);

/**
 * \fn frame_t * getFrame()
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
frame_t * getFrame();

/**
 * \fn int freeFrame(frame_t *frame)
 *
 * 		\brief Brief.
 *
 * 		\param frame ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int freeFrame(frame_t *frame);

/**
 * \fn void setFramePresence(frame_t *frame, int state)
 *
 * 		\brief Brief.
 *
 * 		\param frame ParamBrief.
 * 		\param state ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void setFramePresence(frame_t *frame, int state);

#endif
