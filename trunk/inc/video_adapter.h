/**
 *	\file video_adapter.h
 *
 *		\brief This file defines the basic functions to connect buffers with 
 *		the video driver.
 *
 *		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */
#ifndef VIDEO_ADAPTER_H_
#define VIDEO_ADAPTER_H_
#include "stdio.h"

void bufferMapper ( FILE *f/*, FILE *att*/ );

/**
 *	\fn void flushStdScreen(void)
 *
 *		\brief Flush the stdout file into the video buffer and
 *		refreshes the screen.
 *
 */
void flushStdScreen(void);

#endif
