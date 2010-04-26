/**
 *	\file file.h
 *		
 *		\brief
 *
 *		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */
#ifndef _FILE_H_
#define _FILE_H_

#include "types.h"

#define MAX_OPEN_FILES 20
#define _READ	0x01
#define _WRITE	0x02

/*
 * Public Function File System Access
 */
int getOpenFilesCount ( void ); 
#endif
