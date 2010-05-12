/**
 *	\file types.h
 *
 *		\brief Definition for system dependent types
 *
 * 		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */

#ifndef _TYPES_H_
#define	_TYPES_H_

#include "defs.h"
#include "config.h"

/*
 *	Type definitions
 */

typedef unsigned char Keycode;

typedef unsigned int size_t;

typedef unsigned int ssize_t;

typedef int pid_t;

typedef int key_t;

typedef int tty_t;

typedef void (pfunc_t)(void *);

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

typedef page_t pageTable_t[PAGE_ENTRIES_PER_TABLE];

typedef struct directory {
	pageTable_t tables[PAGE_TABLES_QTY];
	pageTable_t pageDirectory;
} directory_t;

typedef struct frameLocation {
	unsigned int table;
	unsigned int pageEntry;
} frameLocation_t;

typedef struct usedMemBitmap {
	int usedPages;
	unsigned int bitmap[BITMAP_SIZE];
} usedMemBitmap_t;

typedef struct frame{
	int assigned;
	unsigned int address;
} frame_t;

typedef struct memArea{
	void *address;
	size_t size;
	char * allocp;
} memArea_t;

typedef frame_t framesTable_t[TOTAL_FRAMES_QTY];

/*
 *	FILE Type Definition
 */
typedef struct {
	int fd;
	char * buffer;
	char * ptr;
	int flag;
	size_t bufferSize;
} FILE;

typedef struct heapStatus{
	int asigment;
	memArea_t mallocMem;
	} heapStatus;

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
	_SYS_SET_ATOM
};

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

enum PROCCESS { 
	TERMINAL, 
	SHELL, 
	SCREEN_SAVER 
};

/*TTY*/
typedef struct stdInTTY{
	Keycode 	*stdInBuffer;
	int 		writeOffset;
}stdInTTY;

typedef struct stdOutTTY{
	Keycode 	*stdOutBuffer;
	Keycode		*begin;
	Keycode		*end;
}stdOutTTY;

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

typedef struct sysTTY{
	int			qtyTTY;
	tty_s		*listTTY[MAX_TTY];
	tty_t		focusTTY;
}sysTTY;


/*
typedef struct TTY{
	tty_t			ttyId;
	Keycode *		inTTY;
	Keycode		 	TerminalBuffer[VIDEO_MEMORY_SIZE];
	Keycode		 	*begin, *end;
	int				hasScrolled;
	pid_t			focusProcess;
}TTY;

typedef struct sysTTY{
	int		qtyTTY;
	TTY		ttys[MAX_TTY];
	tty_t	focusTTY;
}sysTTY;

typedef struct stdInTTY{
	Keycode stdIn[MAX_LINE];
	int 	writeOffset;
}stdInTTY;
*/

/*SHEL */

typedef void (*shellFuncT)(char *);

typedef struct {
	char * command;
	shellFuncT func;
	char * helpMsg;
} commandT;

typedef int (*pFuncT)(char*);

typedef struct {
	char * name;
	pFuncT func;
	char * helpMsg;
} propertyT;

typedef struct{
	pid_t pid;
	int status;
	char *lineBuffer;
	char *enteredCommand;
	int index;
}dataSlotShell;

typedef dataSlotShell* dataShell;

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;


/* Descriptor de interrupcion */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;

#endif
