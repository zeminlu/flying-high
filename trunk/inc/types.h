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
 * 		\brief Keycode typedef.
 */
typedef unsigned char Keycode;

/**
 * \var size_t
 * 		\brief size_t typedef.
 */
typedef unsigned int size_t;

/**
 * \var ssize_t
 * 		\brief ssize_t typedef.
 */
typedef unsigned int ssize_t;

/**
 * \var pid_t
 * 		\brief pid_t typedef.
 */
typedef int pid_t;

/**
 * \var key_t
 * 		\brief key_t typedef.
 */
typedef int key_t;

/**
 * \var tty_t
 * 		\brief tty_t typedef.
 */
typedef int tty_t;

/**
 * \var pfunc_t
 * 		\brief pfunc_t typedef.
 */
typedef void (pfunc_t)(void *);

/**
 * \struct page_t
 * 		\brief The paging page struct.
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
 * 		\brief The page table typedef.
 */
typedef page_t pageTable_t[PAGE_ENTRIES_PER_TABLE];

/**
 * \struct directory_t
 * 		\brief The page directory typedef.
 */
typedef struct directory {
	pageTable_t tables[PAGE_TABLES_QTY];
	pageTable_t pageDirectory;
} directory_t;

/**
 * \struct frame_t
 * 		\brief The page frame struct.
 */
typedef struct frame{
	int assigned;
	unsigned int address;
} frame_t;

/**
 * \struct memArea_t
 * 		\brief The mem area malloc struct.
 */
typedef struct memArea{
	void *address;
	size_t size;
	char * allocp;
} memArea_t;

/**
 * \struct framesTable_t
 * 		\brief The frames table typedef.
 */
typedef frame_t framesTable_t[TOTAL_FRAMES_QTY];

/**
 * \struct FILE
 * 		\brief The FILE struct.
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
 * 		\brief The heap status struct.
 */
typedef struct heapStatus{
	int asigment;
	memArea_t mallocMem;
	} heapStatus;

/**
 * \struct process_t
 * 		\brief The process struct.
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
 * 		\brief The Syscall id's enum.
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
 * 		\brief The process states enum.
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

/**
 * \struct stdInTTY
 * 		\brief The tty stdin struct.
 */
typedef struct stdInTTY{
	Keycode 	*stdInBuffer;
	int 		writeOffset;
}stdInTTY;

/**
 * \struct stdOutTTY
 * 		\brief The tty stdout struct.
 */
typedef struct stdOutTTY{
	Keycode 	*stdOutBuffer;
	Keycode		*begin;
	Keycode		*end;
}stdOutTTY;

/**
 * \struct tty_s
 * 		\brief The tty_s struct.
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
 * 		\brief The tty system struct.
 */
typedef struct sysTTY{
	int			qtyTTY;
	tty_s		*listTTY[MAX_TTY];
	tty_t		focusTTY;
}sysTTY;

/*SHELL */

/**
 * \var shellFuncT
 * 		\brief The shellFuncT typedef.
 */
typedef void (*shellFuncT)(char *);

/**
 * \struct commandT
 * 		\brief The command struct.
 */
typedef struct {
	char * command;
	shellFuncT func;
	char * helpMsg;
} commandT;

/**
 * \var pFuncT
 * 		\brief the pFuncT typedef.
 */
typedef int (*pFuncT)(char*);

/**
 * \struct propertyT
 * 		\brief The property struct.
 */
typedef struct {
	char * name;
	pFuncT func;
	char * helpMsg;
} propertyT;

/**
 * \struct dataSlotShell
 * 		\brief The shell data slot struct.
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
 * 		\brief The shell data struct typedef.
 */
typedef dataSlotShell* dataShell;

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
/**
 * \struct DESCR_SEG
 * 		\brief OS segment descriptor struct.
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
 * 		\brief OS interrupt descriptor struct.
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
 * 		\brief OS IDTR struct.
 */
typedef struct {
  word  limit;
  dword base;
} IDTR;

#endif
