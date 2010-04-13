/*
 *	Function: fdTable
 *	Author: Santiago Bermudez, Santiago Perez De Rosso, Lucas Pizzagalli
 */

/*
 *	Includes
 */
#include "fdTable.h"

extern unsigned char screenBufAt[];
unsigned char tty0[25*80*2+2];
unsigned char tty1[25*80*2+2];
unsigned char tty2[25*80*2+2];
unsigned char tty3[25*80*2+2];
unsigned char tty4[25*80*2+2];
unsigned char tty5[25*80*2+2];



int SIZE_OF_FDELEM = SIZE_OF_FDELEM_D;

int SIZE_OF_FDELEM_UNIT=SIZE_OF_FDELEM_UNIT_D;
int IO_FLAG=IO_FLAG_D;
int M_FLAG=M_FLAG_D;

/*
 *	File descriptor table
 */
fd_t fdTable[10] = {
	{ (void *)0xB8000 , 10 , 25*80*2+2, M_FLAG_D},
	{ &screenBufAt, 10, 25*80*2+2, M_FLAG_D},
/*	{ &tty0, 10, 25*80*2+2, M_FLAG_D},
	{ &tty1, 10, 25*80*2+2, M_FLAG_D},
	{ &tty2, 10, 25*80*2+2, M_FLAG_D},
	{ &tty3, 10, 25*80*2+2, M_FLAG_D},
	{ &tty4, 10, 25*80*2+2, M_FLAG_D},
	{ &tty5, 10, 25*80*2+2, M_FLAG_D}
*/

};
