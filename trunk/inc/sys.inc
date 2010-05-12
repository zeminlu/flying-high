NULL	equ	0x0

;
; System Calls
;
_SYS_WRITE		equ 0
_SYS_READ		equ	1
_SYS_MEMMAP 	equ 2
_SYS_CREATE		equ 3
_SYS_EXIT		equ 4
_SYS_SET_TTY	equ 5	
_SYS_GET_TTY	equ 6
_SYS_GET_PID	equ 7
_SYS_GET_PPID	equ 8
_SYS_WAIT_PID	equ 9
_SYS_KILL		equ 10
_SYS_FREAD		equ 11
_SYS_FWRITE		equ	12
_SYS_GET_TTY_FP equ 13
_SYS_SET_TTY_FP equ 14
_SYS_SHMGET		equ 15
_SYS_SHMAT		equ 16
_SYS_SHM_DETACH equ 17
_SYS_SEM_GET	equ 18
_SYS_SEM_FREE	equ 19
_SYS_SEM_WAIT	equ 20
_SYS_SEM_SIGNAL	equ 21
_SYS_WAIT		equ 22
_SYS_SET_LEVEL	equ 23
_SYS_GET_LEVEL	equ 24
_SYS_GET_PRIO	equ 25
_SYS_SET_PRIO	equ 26
_SYS_TIME		equ 27
_SYS_SET_ATOM	equ 28
_SYS_UPTIME		equ 29

;
; Standard File Descriptors
;
STDIN	equ 0
STDOUT	equ	1
IN_ATT	equ 2

;
; File Table
;
extern fileSystem
MAX_OPEN_FILES	equ	2
_FS_FILE_SIZE	equ 24
_READ equ 	0x01
_WRITE equ 	0x02

;
; Globals & Externs
;
GLOBAL returnAddress
GLOBAL forceMultitasker

GLOBAL _int_80_handler
GLOBAL _int_08_handler
GLOBAL _int_74_handler
GLOBAL _int_09_handler

GLOBAL _divide_e_hand
GLOBAL _debug_e_hand
GLOBAL _nmi_e_hand
GLOBAL _break_point_e_hand
GLOBAL _overflow_e_hand
GLOBAL _bounds_check_e_hand
GLOBAL _invalid_opcode_e_hand
GLOBAL _copro_unavailable_e_hand
GLOBAL _double_fault_e_hand
GLOBAL _invalid_tss_e_hand
GLOBAL _segment_not_present_e_hand
GLOBAL _stack_e_hand
GLOBAL _general_protection_e_hand
GLOBAL _page_fault_e_hand
GLOBAL _coprocessor_e_hand

EXTERN divideExceptionHandler
EXTERN debugExceptionHandler
EXTERN NMIExceptionHandler
EXTERN breakPointExceptionHandler
EXTERN overflowExceptionHandler
EXTERN boundsCheckExceptionHandler
EXTERN invalidOpcodeExceptionHandler
EXTERN coprocessorUnavailableExceptionHandler
EXTERN doubleFaultExceptionHandler
EXTERN invalidTSSExceptionHandler
EXTERN segmentNotPresentExceptionHandler
EXTERN stackExceptionHandler
EXTERN generalProtectionExceptionHandler
EXTERN pageFaultExceptionHandler
EXTERN coprocessorExceptionHandler

EXTERN increaseKernelDepth
EXTERN decreaseKernelDepth

EXTERN tickCount

EXTERN timerTick
EXTERN keyboard_driver
EXTERN _sys_write
EXTERN _sys_read
EXTERN _sys_fwrite
EXTERN _sys_fread
EXTERN _sys_create_process
EXTERN _sys_memmap
EXTERN _sys_get_tty
EXTERN _sys_set_tty
EXTERN sysGetTTYFocusedProcess
EXTERN sysSetTTYFocusedProcess
EXTERN _sys_get_pid
EXTERN _sys_get_ppid
EXTERN _sys_kill
EXTERN _sys_wait
EXTERN _sys_wait_pid
EXTERN _sys_set_level
EXTERN _sys_get_level
EXTERN _sys_set_priority
EXTERN _sys_get_priority
EXTERN _sys_time
EXTERN _sys_uptime
EXTERN _sys_set_atomicity
EXTERN multitasker
EXTERN _sysExit	
EXTERN refresh
EXTERN loadCR3
EXTERN refreshTTY
EXTERN _sys_shmget
EXTERN _sys_shmat
EXTERN _sys_shm_detach
EXTERN _sys_sem_get
EXTERN _sys_sem_free
EXTERN _sys_sem_wait
EXTERN _sys_sem_signal

EXTERN _debug