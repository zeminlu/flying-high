/*
 *	File: fdTable.h
 *	Author: Santiago Bermudez, Santiago Perez De Rosso, Lucas Pizzagalli
 *	Description: Defines the file descriptor structure
 */
#ifndef FDTABLE_H_
#define FDTABLE_H_
/*
 * 	Defines
 */
#define SIZE_OF_FDELEM_D 4*sizeof(int)
#define SIZE_OF_FDELEM_UNIT_D sizeof(int)
#define IO_FLAG_D 1
#define M_FLAG_D !(IO_FLAG_D)

typedef struct fd_t{
	void *base;
	int offset;
	int limit;
	int flag;
}fd_t;

#endif
