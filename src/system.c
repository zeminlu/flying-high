/*
 *	File system.c
 *
 *		Brief:
 *
 *		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */
#include "system.h"

void rebootSystem(void)
{
	unsigned char good = 0x02;
	while ((good & 0x02) != 0)
        good = inportb(0x64);
    outportb(0x64, 0xFE);
}