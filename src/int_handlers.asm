;------------------------------------------------------------------------------;
;	File: int_handlers.asm													   ;
;																			   ;
;		Brief: Implementation of all interrupts								   ;
;																			   ;
;		Author: Guido Marucci Blas, Nicolas Purita  & Luciano Zemin			   ;
;------------------------------------------------------------------------------;
section .text

%include "sys.inc"

GLOBAL returnAddress
GLOBAL forceMultitasker

global _int_80_handler
global _int_08_handler
global _int_74_handler
global _int_09_handler

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

;extern mouseDriver
extern timerTick
extern keyboard_driver
extern _sys_write
extern _sys_read
extern _sys_create_process
extern _sys_memmap
extern multitasker
extern _sysExit	
extern refresh
extern loadCR3
extern refreshTTY
extern _debug

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
;	call	refreshTTY				;										   ;
;	call	_debug					;										   ;
;	call	multitasker				;										   ;
									;										   ;
returnAddress:						;										   ;
;	call	_debug					;										   ;
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
	jnz		__check_SYS_CREATE		;		break;							   ;
	push	edx						;										   ;
	push	ecx						;										   ;
	push	ebx						;										   ;
	call	_sys_read				;										   ;
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
	jnz		__int_80_ret			;										   ;
	push	ebx						;										   ;
	call	_sys_memmap				;										   ;
	add		esp, 4					;										   ;
	jmp		__int_80_ret			;										   ;
									;	default:							   ;
__int_80_ret:						;		break:							   ;
	pushad
	call	decreaseKernelDepth
	popad
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