;------------------------------------------------------------------------------;
;	File: int_handlers.asm													   ;
;																			   ;
;		Brief: Implementation of all interrupts								   ;
;																			   ;
;		Author: Guido Marucci Blas, Nicolas Purita  & Luciano Zemin			   ;
;------------------------------------------------------------------------------;
section .text

%include "sys.inc"

forceMultitasker:
	push 	ebp
	mov		ebp, esp
	int		08h
	leave
	ret
	
;------------------------------------------------------------------------------;
;_int_08_handler															   ;
;																			   ;
;	Brief: It handles the timer tick interrupt and calls the timer tick		   ;
;	function.																   ;
;																			   ;
;																			   ;
;------------------------------------------------------------------------------;
_int_08_handler:					;										   ;
	cli								;										   ;
	pushad							;										   ;
	call 	increaseKernelDepth		;										   ;
	call	refreshTTY				;										   ;
	call	multitasker				;										   ;
									;										   ;
returnAddress:						;										   ;
	mov 	al, 20h					;										   ;
	out 	20h, al					;										   ;
	call	decreaseKernelDepth		;										   ;
	call	loadCR3					;										   ;
	popad							;										   ;
	iret							;										   ;
;------------------------------------------------------------------------------;

;------------------------------------------------------------------------------;
; _int_80_handler															   ;
;																			   ;
;	Brief: It dispatches the required system call							   ;
;	sent in the eax register. If an invalid value							   ;
;	was sent in eax, the function does nothing								   ;
;																			   ;
;	Parameters: All parameters are recived from right 						   ;
;	to left in this order													   ;
;	ebx ecx edx ebp edi esi													   ;
;																			   ;
;	Returns: the value returned by the system call or						   ;
;	the invalid value if such was given.									   ;
;------------------------------------------------------------------------------;
_int_80_handler:					;										   ;
	cli
	pushad
	call	increaseKernelDepth		;										   ;
	popad
	
									;										   ;
__check_SYS_WRITE:					; switch(eax) {							   ;
	cmp		eax, _SYS_WRITE			;	case _SYS_WRITE:					   ;
	jnz		__check_SYS_READ		;		_sys_write(ebx, ecx, edx);		   ;
	push	edx						;		break;							   ;
	push	ecx						;										   ;
	push	ebx						;										   ;
	call	_sys_write				;										   ;
	add		esp, 12					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_READ:					;	case _SYS_READ:						   ;
	cmp		eax, _SYS_READ			;		_sys_read(ebx, ecx, eds);		   ;
	jnz		__check_SYS_FWRITE		;		break;							   ;
	push	edx						;										   ;
	push	ecx						;										   ;
	push	ebx						;										   ;
	call	_sys_read				;										   ;
	add		esp,12					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_FWRITE:					; switch(eax) {							   ;
	cmp		eax, _SYS_FWRITE		;	case _SYS_WRITE:					   ;
	jnz		__check_SYS_FREAD		;		_sys_write(ebx, ecx, edx);		   ;
	push	edx						;		break;							   ;
	push	ecx						;										   ;
	push	ebx						;										   ;
	call	_sys_fwrite				;										   ;
	add		esp, 12					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_FREAD:					;	case _SYS_READ:						   ;
	cmp		eax, _SYS_FREAD			;		_sys_read(ebx, ecx, eds);		   ;
	jnz		__check_SYS_CREATE		;		break;							   ;
	push	edx						;										   ;
	push	ecx						;										   ;
	push	ebx						;										   ;
	call	_sys_fread				;										   ;
	add		esp,12					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_CREATE:					;										   ;
	cmp 	eax, _SYS_CREATE		;										   ;
	jnz		__check_SYS_EXIT		;										   ;
	push	ESI						;										   ;
	push	EDI						;										   ;
	push	EDX						;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_create_process		;										   ;
	add		esp, 20					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_EXIT:					;										   ;
	cmp		eax, _SYS_EXIT			;										   ;
	jnz		__check_SYS_MEMMAP		;										   ;
	push 	EBX						;										   ;
	call 	_sysExit				;										   ;
	add		esp, 4					;										   ;
	jmp 	__int_80_ret			;										   ;
									;										   ;
__check_SYS_MEMMAP:					;										   ;
	cmp		eax, _SYS_MEMMAP		;										   ;
	jnz		__check_SYS_SET_TTY		;										   ;
	push	ebx						;										   ;
	call	_sys_memmap				;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_SET_TTY:				;										   ;
	cmp 	eax, _SYS_SET_TTY		;										   ;
	jnz		__check_SYS_GET_TTY		;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_set_tty			;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_GET_TTY:				;										   ;
	cmp 	eax, _SYS_GET_TTY		;										   ;
	jnz		__check_SYS_GET_PID		;										   ;
	push	EBX						;										   ;
	call	_sys_get_tty			;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;
				
__check_SYS_GET_TTY_FP:				;										   ;
	cmp 	eax, _SYS_GET_TTY_FP	;										   ;
	jnz		__check_SYS_SET_TTY_FP	;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	sysGetTTYFocusedProcess	;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SET_TTY_FP:				;										   ;
	cmp 	eax, _SYS_SET_TTY_FP	;										   ;
	jnz		__check_SYS_GET_PID		;										   ;
	push	EBX						;										   ;
	call	sysSetTTYFocusedProcess	;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_GET_PID:				;										   ;
	cmp 	eax, _SYS_GET_PID		;										   ;
	jnz		__check_SYS_GET_PPID	;										   ;
	call	_sys_get_pid			;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_GET_PPID:				;										   ;
	cmp 	eax, _SYS_GET_PPID		;										   ;
	jnz		__check_SYS_WAIT		;										   ;
	push	EBX						;										   ;
	call	_sys_get_ppid			;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_WAIT:					;										   ;
	cmp 	eax, _SYS_WAIT			;										   ;
	jnz		__check_SYS_WAIT_PID	;										   ;
	push	EBX						;										   ;
	call	_sys_wait				;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_WAIT_PID:				;										   ;
	cmp 	eax, _SYS_WAIT_PID		;										   ;
	jnz		__check_SYS_KILL		;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_wait_pid			;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__check_SYS_KILL:					;										   ;
	cmp 	eax, _SYS_KILL			;										   ;
	jnz		__check_SYS_SET_LEVEL	;										   ;
	push	EBX						;										   ;
	call	_sys_kill				;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SET_LEVEL:				;										   ;
	cmp 	eax, _SYS_SET_LEVEL		;										   ;
	jnz		__check_SYS_GET_LEVEL	;										   ;
	push	EBX						;										   ;
	call	_sys_set_level			;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;
__check_SYS_GET_LEVEL:				;										   ;
	cmp 	eax, _SYS_GET_LEVEL		;										   ;
	jnz		__check_SYS_GET_PRIO	;										   ;
	call	_sys_get_level			;										   ;
	jmp		__int_80_ret			;										   ;
__check_SYS_GET_PRIO:				;										   ;
	cmp 	eax, _SYS_GET_PRIO		;										   ;
	jnz		__check_SYS_SET_PRIO	;										   ;
	call	_sys_get_priority		;										   ;
	jmp		__int_80_ret			;										   ;
__check_SYS_SET_PRIO:				;										   ;
	cmp 	eax, _SYS_SET_PRIO		;										   ;
	jnz		__check_SYS_TIME		;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_set_priority		;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;
__check_SYS_TIME:					;										   ;
	cmp 	eax, _SYS_TIME			;										   ;
	jnz		__check_SYS_SET_ATOM	;										   ;
	call	_sys_time				;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SET_ATOM:				;										   ;
	cmp 	eax, _SYS_SET_ATOM		;										   ;
	jnz		__check_SYS_SHMGET		;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_set_atomicity		;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SHMGET:					;										   ;
	cmp 	eax, _SYS_SHMGET		;										   ;
	jnz		__check_SYS_SHMAT		;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_shmget				;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SHMAT:					;										   ;
	cmp 	eax, _SYS_SHMAT			;										   ;
	jnz		__check_SYS_SHM_DETACH	;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_shmat				;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SHM_DETACH:				;										   ;
	cmp 	eax, _SYS_SHM_DETACH	;										   ;
	jnz		__check_SYS_SEM_GET		;										   ;
	push	EBX						;										   ;
	call	_sys_shm_detach			;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SEM_GET:				;										   ;
	cmp 	eax, _SYS_SEM_GET		;										   ;
	jnz		__check_SYS_SEM_FREE	;										   ;
	call	_sys_sem_get			;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SEM_FREE:				;										   ;
	cmp 	eax, _SYS_SEM_FREE		;										   ;
	jnz		__check_SYS_SEM_WAIT	;										   ;
	push	EBX						;										   ;
	call	_sys_sem_free			;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SEM_WAIT:				;										   ;
	cmp 	eax, _SYS_SEM_WAIT		;										   ;
	jnz		__check_SYS_SEM_SIGNAL	;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_sem_wait			;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;

__check_SYS_SEM_SIGNAL:				;										   ;
	cmp 	eax, _SYS_SEM_SIGNAL	;										   ;
	jnz		__int_80_ret			;										   ;
	push	ECX						;										   ;
	push	EBX						;										   ;
	call	_sys_sem_signal			;										   ;
	add		esp, 8					;										   ;
	jmp		__int_80_ret			;										   ;
									;										   ;
__int_80_ret:						;										   ;
	pushad							;										   ;
	call	decreaseKernelDepth		;										   ;
	popad							;										   ;
	iret							;										   ;
;------------------------------------------------------------------------------;

;------------------------------------------------------------------------------;
; _int_74_handler                                                              ;
;                                                                              ;
;   Brief: It handles the interrupt of the mouse (IRQ 12) by calling the       ;
;   mouse driver.															   ;
;------------------------------------------------------------------------------;
;_int_74_handler:                    ;                                          ;
;    push    ebp                     ; Building Stack Frame                     ;
;	mov     ebp, esp                ;                                          ;
;	and		esp, -16				;										   ;
;	pusha                           ; 										   ;                
;									;										   ;
;	call    mouseDriver				;										   ;
;	mov		al, 0x20				; Sending generic EOI to PIC 1 & 2		   ;
;	out		0xA0, al                ;										   ;
;	out		0x20, al				;										   ;
;									;										   ;
;	popa							;										   ;
;	leave							;										   ;
;	iret							;									       ;
;------------------------------------------------------------------------------;

;------------------------------------------------------------------------------;
; _int_09_handler															   ;
;																			   ;
;	Brief: 	This is the routine to manipulate the interrupt number 9. Then call;
;			to the															   ;
;			keyboard_driver that it's implemented in C.						   ;
; -----------------------------------------------------------------------------;
_int_09_handler:						; Building the stack Frame			   ;
	cli
	pushad                     	    	; 									   ;
	call	increaseKernelDepth			;									   ;

	call    keyboard_driver    		    ; Call to the keyboard_driver (imple.C);
	mov		al,20h						; Send EOI							   ;
	out		20h,al						; Write the instruction in port 0x20   ;

	call	decreaseKernelDepth			;									   ;
	popad								; Leave the Stack Frame				   ;
	iret								;									   ;
; ---------------------------------------------------------------------------- ;

;-----------------------------------------------------------------------------;
; Function: _devide_e_hand													  ;
; Description:																  ;
;		Handles the divide by zero exception.								  ;
; 																			  ;
; Programmer: Luciano Zemin											  ;
;-----------------------------------------------------------------------------;
_divide_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call divideExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _debug_e_hand													  ;
; Description:																  ;
;		Handles the debug exception.										  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_debug_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call debugExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _nmi_e_hand														  ;
; Description:																  ;
;		Handles the non maskable exception.									  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_nmi_e_hand:
	cli
	pushad

	call increaseKernelDepth	
	call NMIExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _break_point_e_hand												  ;
; Description:																  ;
;		Handles the break point exception.									  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_break_point_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call breakPointExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _overflow_e_hand												  ;
; Description:																  ;
;		Handles the overflow exception.										  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_overflow_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call overflowExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _bounds_check_e_hand											  ;
; Description:																  ;
;		Handles the bounds check exception.									  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_bounds_check_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call boundsCheckExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _invalid_opcode_e_hand											  ;
; Description:																  ;
;		Handles the invalid opcode exception.								  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_invalid_opcode_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call invalidOpcodeExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _copro_unavailable_e_hand										  ;
; Description:																  ;
;		Handles the coprocessor unavailable exception.						  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_copro_unavailable_e_hand:
	cli
	pushad
	
	call increaseKernelDepth
	call coprocessorUnavailableExceptionHandler	
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _double_fault_e_hand											  ;
; Description:																  ;
;		Handles the double fault exception.									  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_double_fault_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call doubleFaultExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _invalid_tss_e_hand			 									  ;
; Description:																  ;
;		Handles the invalid TSS exception.									  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_invalid_tss_e_hand:
	cli
	pushad
	
	call increaseKernelDepth
	call invalidTSSExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _segment_not_present_e_hand										  ;
; Description:																  ;
;		Handles the segment not present exception.							  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_segment_not_present_e_hand:
	cli
	pushad
	
	call increaseKernelDepth
	call segmentNotPresentExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _stack_e_hand													  ;
; Description:																  ;
;		Handles the stack exception.										  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_stack_e_hand:
	cli
	pushad
	
	call increaseKernelDepth
	call stackExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _general_protection_e_hand										  ;
; Description:																  ;
;		Handles the general protection exception.							  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_general_protection_e_hand:
	cli
	pushad
	
	call increaseKernelDepth
	call generalProtectionExceptionHandler
	call decreaseKernelDepth
	
	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _page_fault_e_hand												  ;
; Description:																  ;
;		Handles the page fault exception.									  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_page_fault_e_hand:
	cli
	pushad
	
	call increaseKernelDepth	
	call pageFaultExceptionHandler
	call decreaseKernelDepth

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _coprocessor_e_hand												  ;
; Description:																  ;
;		Handles the coprocessor exception.									  ;
; 																			  ;
; Programmer: Luciano Zemin													  ;
;-----------------------------------------------------------------------------;
_coprocessor_e_hand:
	cli
	pushad

	call increaseKernelDepth
	call coprocessorExceptionHandler
	call decreaseKernelDepth

	popad
	iret