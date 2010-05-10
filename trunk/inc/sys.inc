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
MAX_OPEN_FILES	equ	4
_FS_FILE_SIZE	equ 24
_READ equ 	0x01
_WRITE equ 	0x02
