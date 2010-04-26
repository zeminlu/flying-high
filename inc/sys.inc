NULL	equ	0x0

;
; System Calls
;
_SYS_WRITE		equ 0
_SYS_READ		equ	1
_SYS_MEMMAP 	equ 2
_SYS_CREATE		equ 3
_SYS_EXIT		equ 4
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
MAX_OPEN_FILES	equ	20
_FS_FILE_SIZE	equ 24
_READ equ 	0x01
_WRITE equ 	0x02
