/**
 *	\file system.c
 *
 *		\brief System reboot module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "system.h"

void rebootSystem(void)
{
	unsigned char good = 0x02;
	while ((good & 0x02) != 0)
        good = inportb(0x64);
    outportb(0x64, 0xFE);
}