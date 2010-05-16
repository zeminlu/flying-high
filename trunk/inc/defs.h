/**
 *	\file defs.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _DEFS_H_
#define _DEFS_H_

/**
 * \def BYTE
 *		\brief DefineBrief.
 */
#define byte unsigned char
/**
 * \def WORD
 *		\brief DefineBrief.
 */
#define word short int
/**
 * \def DWORD
 *		\brief DefineBrief.
 */
#define dword int
/**
 * \def TRUE
 *		\brief DefineBrief.
 */
#define TRUE 1
/**
 * \def FALSE
 *		\brief DefineBrief.
 */
#define FALSE !TRUE
/**
 * \def OK
 *		\brief DefineBrief.
 */
#define OK	TRUE
/**
 * \def ERROR
 *		\brief DefineBrief.
 */
#define	ERROR FALSE

/**
 * \def _READ
 *		\brief DefineBrief.
 */
#define _READ	0x01
/**
 * \def _WRITE
 *		\brief DefineBrief.
 */
#define _WRITE	0x02

/**
 * \def SEND_TO_VIDEO
 *		\brief DefineBrief.
 */
#define	SEND_TO_VIDEO	1
/**
 * \def WRITE_ON_TTY
 *		\brief DefineBrief.
 */
#define WRITE_ON_TTY	0

/**
 * \def TTY_RAW
 *		\brief DefineBrief.
 */
#define	TTY_RAW			0
/**
 * \def TTY_CANONICAL
 *		\brief DefineBrief.
 */
#define	TTY_CANONICAL	1

/**
 * \def LOAD
 *		\brief DefineBrief.
 */
#define	LOAD	0
/**
 * \def SAVE
 *		\brief DefineBrief.
 */
#define	SAVE	1

/**
 * \def STDIN
 *		\brief DefineBrief.
 */
#define STDIN 	0
/**
 * \def STDOUT
 *		\brief DefineBrief.
 */
#define STDOUT 	1
/**
 * \def IN_ATT
 *		\brief DefineBrief.
 */
#define IN_ATT	2
/**
 * \def OUT_ATT
 *		\brief DefineBrief.
 */
#define OUT_ATT 3

/**
 * \def ATOMIC
 *		\brief DefineBrief.
 */
#define	ATOMIC			1
/**
 * \def UNATOMIC
 *		\brief DefineBrief.
 */
#define UNATOMIC		!ATOMIC
/**
 * \def FOREGROUND
 *		\brief DefineBrief.
 */
#define	FOREGROUND		0
/**
 * \def BACKGROUND
 *		\brief DefineBrief.
 */
#define	BACKGROUND		1

/*	Units Definitions */
/**
 * \def KB
 *		\brief DefineBrief.
 */
#define KB							* (1 << 10)
/**
 * \def MB
 *		\brief DefineBrief.
 */
#define MB							* (1 << 20)

/* Memory Space Model */
/**
 * \def KERNEL_SPACE
 *		\brief DefineBrief.
 */
#define KERNEL_SPACE				0x00000000
/**
 * \def KERNEL_SPACE_SIZE
 *		\brief DefineBrief.
 */
#define KERNEL_SPACE_SIZE			(8 MB)
/**
 * \def MALLOC_SPACE
 *		\brief DefineBrief.
 */
#define MALLOC_SPACE				(KERNEL_SPACE + KERNEL_SPACE_SIZE)
/**
 * \def MALLOC_SPACE_SIZE
 *		\brief DefineBrief.
 */
#define MALLOC_SPACE_SIZE			(8 MB)
/**
 * \def TOTAL_MEMORY_SIZE
 *		\brief DefineBrief.
 */
#define TOTAL_MEMORY_SIZE			(KERNEL_SPACE_SIZE + MALLOC_SPACE_SIZE)

/* Page Definitions */
/**
 * \def PAGE_SIZE
 *		\brief DefineBrief.
 */
#define PAGE_SIZE					(4 KB)
/**
 * \def PAGE_ENTRY_SIZE
 *		\brief DefineBrief.
 */
#define PAGE_ENTRY_SIZE				sizeof(unsigned int)
/**
 * \def PAGE_ENTRIES_PER_TABLE
 *		\brief DefineBrief.
 */
#define PAGE_ENTRIES_PER_TABLE		(PAGE_SIZE / PAGE_ENTRY_SIZE)
/**
 * \def KERNEL_TABLES_QTY
 *		\brief DefineBrief.
 */
#define KERNEL_TABLES_QTY			(KERNEL_SPACE_SIZE / (PAGE_SIZE * PAGE_ENTRIES_PER_TABLE))
/**
 * \def MALLOC_TABLES_QTY
 *		\brief DefineBrief.
 */
#define MALLOC_TABLES_QTY			(MALLOC_SPACE_SIZE / (PAGE_SIZE * PAGE_ENTRIES_PER_TABLE))
/**
 * \def PAGE_TABLES_QTY
 *		\brief DefineBrief.
 */
#define PAGE_TABLES_QTY				(KERNEL_TABLES_QTY + MALLOC_TABLES_QTY)

/* Frame Definitions */
/**
 * \def FRAMES_PER_TABLE
 *		\brief DefineBrief.
 */
#define FRAMES_PER_TABLE			(PAGE_ENTRIES_PER_TABLE / PAGES_PER_FRAME)
/**
 * \def TOTAL_FRAMES_QTY
 *		\brief DefineBrief.
 */
#define TOTAL_FRAMES_QTY			(MALLOC_TABLES_QTY * FRAMES_PER_TABLE)

/* Table's Index Definitions */
/**
 * \def FIRST_KERNEL_TABLE
 *		\brief DefineBrief.
 */
#define FIRST_KERNEL_TABLE			0
/**
 * \def FIRST_MALLOC_TABLE
 *		\brief DefineBrief.
 */
#define FIRST_MALLOC_TABLE			FIRST_KERNEL_TABLE + KERNEL_TABLES_QTY

/* Free Memory Bitmap Definitions */
/**
 * \def PAGES_PER_INT
 *		\brief DefineBrief.
 */
#define PAGES_PER_INT				((int)(sizeof(unsigned int)) * 8)
/**
 * \def INTS_PER_TABLE
 *		\brief DefineBrief.
 */
#define INTS_PER_TABLE				(PAGE_ENTRIES_PER_TABLE / 32)
/**
 * \def BITMAP_SIZE
 *		\brief DefineBrief.
 */
#define BITMAP_SIZE					(INTS_PER_TABLE * MALLOC_TABLES_QTY)
/**
 * \def MAX_MAPPED_ADDRESS
 *		\brief DefineBrief.
 */
#define MAX_MAPPED_ADDRESS			(TOTAL_MEMORY_SIZE - 1)
/**
 * \def getBitmapIndex(addr)
 *		\brief DefineBrief.
 */
#define getBitmapIndex(addr)		(((addr) / (unsigned)PAGE_SIZE) / (unsigned)PAGES_PER_INT)
/**
 * \def getBitmapOffset(addr)
 *		\brief DefineBrief.
 */
#define getBitmapOffset(addr)		(0x00000001 << (((addr) / PAGE_SIZE) % PAGES_PER_INT))
/**
 * \def OFFSET_MASK
 *		\brief DefineBrief.
 */
#define OFFSET_MASK					0xFFFFF000

/* Flags para derechos de acceso de los segmentos */
/**
 * \def ACS_PRESENT
 *		\brief DefineBrief.
 */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
/**
 * \def ACS_CSEG
 *		\brief DefineBrief.
 */
#define ACS_CSEG        0x18            /* segmento de codigo */
/**
 * \def ACS_DSEG
 *		\brief DefineBrief.
 */
#define ACS_DSEG        0x10            /* segmento de datos */
/**
 * \def ACS_READ
 *		\brief DefineBrief.
 */
#define ACS_READ        0x02            /* segmento de lectura */
/**
 * \def ACS_WRITE
 *		\brief DefineBrief.
 */
#define ACS_WRITE       0x02            /* segmento de escritura */
/**
 * \def ACS_IDT
 *		\brief DefineBrief.
 */
#define ACS_IDT         ACS_DSEG
/**
 * \def ACS_INT_386
 *		\brief DefineBrief.
 */
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
/**
 * \def ACS_INT
 *		\brief DefineBrief.
 */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )

/**
 * \def ACS_CODE
 *		\brief DefineBrief.
 */
#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
/**
 * \def ACS_DATA
 *		\brief DefineBrief.
 */
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
/**
 * \def ACS_STACK
 *		\brief DefineBrief.
 */
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

/**
 * \def NULL
 *		\brief NULL pointer definition.
 */#define NULL (void *)0x0

#endif