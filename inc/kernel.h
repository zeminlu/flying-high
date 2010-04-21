#include "defs.h"

/********************************** 
*
*  Kernel
*
***********************************/

#ifndef _KERNEL_H_
#define _KERNEL_H_

#define OS_PID	0

int (*player)(void);

typedef int size_t;
typedef short int ssize_t;
typedef enum eINT_80 {WRITE=0, READ} tINT_80;
typedef enum eUSER {U_KERNEL=0, U_NORMAL} tUSERS;

#endif
