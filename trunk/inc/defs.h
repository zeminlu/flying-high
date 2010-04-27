/***************************************************
  Defs.h
	
****************************************************/

#ifndef _DEFS_H_
#define _DEFS_H_

#define byte unsigned char
#define word short int
#define dword int
#define TRUE 1
#define FALSE !TRUE
#define OK	TRUE
#define	ERROR FALSE

#define	SEND_TO_VIDEO	1
#define WRITE_ON_TTY	0

#define STDIN 	0
#define STDOUT 	1
#define IN_ATT	2
#define OUT_ATT 3

#define	ATOMIC			1
#define UNATOMIC		!ATOMIC
#define RUNNING			0
#define READY			1
#define BLOCKED			2
#define WAITING_CHILD	3
#define WAITING_TIME	4
#define WAITING_PID		5
#define TERMINATED		6
#define ZOMBIE			7
#define	FOREGROUND		0
#define	BACKGROUND		1
#define MAX_FILES		3

/*	Units Definitions */
#define KB							* (1 << 10)
#define MB							* (1 << 20)

/* Memory Space Model */
#define KERNEL_SPACE				0x00000000
#define KERNEL_SPACE_SIZE			(8 MB)
#define MALLOC_SPACE				(KERNEL_SPACE + KERNEL_SPACE_SIZE)
#define MALLOC_SPACE_SIZE			(8 MB)
#define TOTAL_MEMORY_SIZE			(KERNEL_SPACE_SIZE + MALLOC_SPACE)

/* Page Definitions */
#define PAGE_SIZE					(4 KB)
#define PAGE_ENTRY_SIZE				sizeof(unsigned int)
#define PAGE_ENTRIES_PER_TABLE		(PAGE_SIZE / PAGE_ENTRY_SIZE)
#define KERNEL_TABLES_QTY			(KERNEL_SPACE_SIZE / (PAGE_SIZE * PAGE_ENTRIES_PER_TABLE))
#define MALLOC_TABLES_QTY			(MALLOC_SPACE_SIZE / (PAGE_SIZE * PAGE_ENTRIES_PER_TABLE))
#define PAGE_TABLES_QTY				(KERNEL_TABLES_QTY + MALLOC_TABLES_QTY)

/* Frame Definitions */
#define PAGES_PER_FRAME				(PAGE_ENTRIES_PER_TABLE / 8)
#define PAGES_PER_HEAP				(PAGES_PER_FRAME - 1)
#define PAGES_PER_STACKFRAME		1
#define FRAMES_PER_TABLE			(PAGE_ENTRIES_PER_TABLE / PAGES_PER_FRAME)
#define TOTAL_FRAMES_QTY			(MALLOC_TABLES_QTY * PAGE_ENTRIES_PER_TABLE)

/* Table's Index Definitions */
#define FIRST_KERNEL_TABLE			0
#define FIRST_MALLOC_TABLE			FIRST_KERNEL_TABLE + KERNEL_TABLES_QTY

/* Free Memory Bitmap Definitions */
#define PAGES_PER_INT				((int)(sizeof(unsigned int)) * 8)
#define INTS_PER_TABLE				(PAGE_ENTRIES_PER_TABLE / 32)
#define BITMAP_SIZE					(INTS_PER_TABLE * MALLOC_TABLES_QTY)
#define MAX_MAPPED_ADDRESS			(TOTAL_MEMORY_SIZE - 1)
#define getBitmapIndex(addr)		(((addr) / (unsigned)PAGE_SIZE) / (unsigned)PAGES_PER_INT)
#define getBitmapOffset(addr)		(0x00000001 << (((addr) / PAGE_SIZE) % PAGES_PER_INT))
#define OFFSET_MASK					0xFFFFF000

/* Kernel Heap Area */
#define KERNEL_HEAP_PAGES_QTY		10

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )


#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

/*
 *	NULL Pointer Definition
 */
#define NULL (void *)0x0

#endif

