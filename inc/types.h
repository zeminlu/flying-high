/**
 *	\file types.h
 *
 *		\brief Definition for system dependent types
 *
 * 		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _TYPES_H_
#define	_TYPES_H_

#include "defs.h"
#include "config.h"

/*
 *	Type definitions
 */

/**
 * \var Keycode
 * 		\brief Brief.
 */
typedef unsigned char Keycode;

/**
 * \var size_t
 * 		\brief Brief.
 */
typedef unsigned int size_t;

/**
 * \var ssize_t
 * 		\brief Brief.
 */
typedef unsigned int ssize_t;

/**
 * \var pid_t
 * 		\brief Brief.
 */
typedef int pid_t;

/**
 * \var key_t
 * 		\brief Brief.
 */
typedef int key_t;

/**
 * \var tty_t
 * 		\brief Brief.
 */
typedef int tty_t;

/**
 * \var pfunc_t
 * 		\brief Brief.
 */
typedef void (pfunc_t)(void *);

/**
 * \struct page_t
 * 		\brief Brief.
 */
typedef struct page {
	unsigned int present 		: 1;
	unsigned int writeable 		: 1;
	unsigned int user 			: 1;
	unsigned int directWrite	: 1;
	unsigned int disableCache	: 1;
	unsigned int accessed		: 1;
	unsigned int dirty			: 1;
	unsigned int reserved		: 5;
	unsigned int address		: 20;
} page_t;

/**
 * \struct pageTable_t
 * 		\brief Brief.
 */
typedef page_t pageTable_t[PAGE_ENTRIES_PER_TABLE];

/**
 * \struct directory_t
 * 		\brief Brief.
 */
typedef struct directory {
	pageTable_t tables[PAGE_TABLES_QTY];
	pageTable_t pageDirectory;
} directory_t;

/**
 * \struct frameLocation_t
 * 		\brief Brief.
 */
typedef struct frameLocation {
	unsigned int table;
	unsigned int pageEntry;
} frameLocation_t;

/**
 * \struct usedMemBitmap_t
 * 		\brief Brief.
 */
typedef struct usedMemBitmap {
	int usedPages;
	unsigned int bitmap[BITMAP_SIZE];
} usedMemBitmap_t;

/**
 * \struct frame_t
 * 		\brief Brief.
 */
typedef struct frame{
	int assigned;
	unsigned int address;
} frame_t;

/**
 * \struct memArea_t
 * 		\brief Brief.
 */
typedef struct memArea{
	void *address;
	size_t size;
	char * allocp;
} memArea_t;

/**
 * \struct framesTable_t
 * 		\brief Brief.
 */
typedef frame_t framesTable_t[TOTAL_FRAMES_QTY];

/*
 *	FILE Type Definition
 */
/**
 * \struct FILE
 * 		\brief Brief.
 */
typedef struct {
	int fd;
	char * buffer;
	char * ptr;
	int flag;
	size_t bufferSize;
} FILE;

/**
 * \struct heapStatus
 * 		\brief Brief.
 */
typedef struct heapStatus{
	int asigment;
	memArea_t mallocMem;
	} heapStatus;

/**
 * \struct process_t
 * 		\brief Brief.
 */
typedef struct process_t {
	pid_t pid;
	pid_t ppid;
	pid_t gid;
	tty_t tty;
	pid_t childs[MAX_CHILDS];
	int childsQty;
	char name[MAX_PROCESS_NAME + 1];
	int state;
	FILE *files[MAX_FILES];
	int rpgPrior;
	int	rpgOld;
	unsigned tickCounter;
	double cpuPercent;
	int priority;
	int level;
	pid_t waitingPid;
	pid_t sleepingPid;
	int waitedStatus;
	void *ebp;
	void *esp;
	void *stack;
	void *heap;
	frame_t *sFrame;
	frame_t *pFrame;
	frame_t *hFrame;
	int atomicity;
	int	ttyMode;
	heapStatus dataUmalloc;
	
} process_t;

/**
 * \enum SYSCALL_IDS
 * 		\brief Brief.
 */
enum SYSCALL_IDS{
	_SYS_WRITE,
	_SYS_READ,
	_SYS_MEMMAP,
	_SYS_CREATE,
	_SYS_EXIT,
	_SYS_SET_TTY,	
	_SYS_GET_TTY,
	_SYS_GET_PID,
	_SYS_GET_PPID,
	_SYS_WAIT_PID,
	_SYS_KILL,
	_SYS_FREAD,
	_SYS_FWRITE,
	_SYS_GET_TTY_FP,
	_SYS_SET_TTY_FP,
	_SYS_SHMGET,
	_SYS_SHMAT,
	_SYS_SHM_DETACH,
	_SYS_SEM_GET,
	_SYS_SEM_FREE,
	_SYS_SEM_WAIT,
	_SYS_SEM_SIGNAL,
	_SYS_WAIT,
	_SYS_SET_LEVEL,
	_SYS_GET_LEVEL,
	_SYS_GET_PRIO,
	_SYS_SET_PRIO,
	_SYS_TIME,
	_SYS_SET_ATOM,
	_SYS_UPTIME,
	_SYS_SET_TTY_MODE,
	_SYS_GET_TTY_MODE
};

/**
 * \enum STATES
 * 		\brief Brief.
 */
enum STATES{
	RUNNING,
	READY,
	BLOCKED,
	WAITING_CHILD,
	WAITING_TIME,
	WAITING_PID,
	TERMINATED,
	DEAD
};

/*TTY*/
/**
 * \struct stdInTTY
 * 		\brief Brief.
 */
typedef struct stdInTTY{
	Keycode 	*stdInBuffer;
	int 		writeOffset;
}stdInTTY;

/**
 * \struct stdOutTTY
 * 		\brief Brief.
 */
typedef struct stdOutTTY{
	Keycode 	*stdOutBuffer;
	Keycode		*begin;
	Keycode		*end;
}stdOutTTY;

/**
 * \struct tty_s
 * 		\brief Brief.
 */
typedef struct tty_s{
	tty_t			ttyId;
	stdInTTY		*stdIn;
	stdOutTTY		*stdOut;
	int				hasScrolled;
	pid_t			focusProcess;
	int				writePointer;
	int				writeCol;
	int				writeRow;
	int				readPointer;
	int				readCol;
	int				readRow;
}tty_s;

/**
 * \struct sysTTY
 * 		\brief Brief.
 */
typedef struct sysTTY{
	int			qtyTTY;
	tty_s		*listTTY[MAX_TTY];
	tty_t		focusTTY;
}sysTTY;

/*SHELL */

/**
 * \var shellFuncT
 * 		\brief Brief.
 */
typedef void (*shellFuncT)(char *);

/**
 * \struct commandT
 * 		\brief Brief.
 */
typedef struct {
	char * command;
	shellFuncT func;
	char * helpMsg;
} commandT;

/**
 * \var pFuncT
 * 		\brief Brief.
 */
typedef int (*pFuncT)(char*);

/**
 * \struct propertyT
 * 		\brief Brief.
 */
typedef struct {
	char * name;
	pFuncT func;
	char * helpMsg;
} propertyT;

/**
 * \struct dataSlotShell
 * 		\brief Brief.
 */
typedef struct{
	pid_t pid;
	int status;
	char *lineBuffer;
	char *enteredCommand;
	int index;
}dataSlotShell;

/**
 * \var dataShell
 * 		\brief Brief.
 */
typedef dataSlotShell* dataShell;

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
/**
 * \struct DESCR_SEG
 * 		\brief Brief.
 */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;


/* Descriptor de interrupcion */
/**
 * \struct DESCR_INT
 * 		\brief Brief.
 */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
/**
 * \struct IDTR
 * 		\brief Brief.
 */
typedef struct {
  word  limit;
  dword base;
} IDTR;

#endif
