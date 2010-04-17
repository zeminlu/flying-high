;------------------------------------------------------------------------------;
;	File: int_handlers.asm													   ;
;																			   ;
;		Brief: Implementation of all interrupts								   ;
;																			   ;
;		Author: Guido Marucci Blas, Nicolas Purita  & Luciano Zemin			   ;
;------------------------------------------------------------------------------;
section .text

%include "sys.inc"

global _int_80_handler
global _int_08_handler
global _int_74_handler
global _int_09_handler

extern mouseDriver
extern timerTick
extern keyboard_driver
extern _sys_write
extern _sys_read
extern _sys_memmap

;------------------------------------------------------------------------------;
;_int_08_handler															   ;
;																			   ;
;	Brief: It handles the timer tick interrupt and calls the timer tick		   ;
;	function.																   ;
;																			   ;
;																			   ;
;------------------------------------------------------------------------------;
_int_08_handler:		;													   ;
	push	ebp			; Building Stack Frame								   ;
	mov		ebp, esp	;													   ;
	and		esp, -16	; Aligning Segment to 16 bytes						   ;
	pusha				; Saving Caller's registers							   ;
						;													   ;
	call	timerTick	; 													   ;
						;													   ;
	mov		al,0x20		; Notifying PIC1 of End OF Interrupt				   ;
	out		0x20,al		;													   ;
						;													   ;
	popa				; Restoring Caller's registers						   ;
	leave				;													   ;
	iret				;													   ;
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
	push	ebp						; Building Stack Frame					   ;
	mov		ebp, esp				;										   ;
	and		esp, -16				; Aligning segment to 16 bytes			   ;
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
	jnz		__check_SYS_MEMMAP		;		break;							   ;
	push	edx						;										   ;
	push	ecx						;										   ;
	push	ebx						;										   ;
	call	_sys_read				;										   ;
	add		esp,12					;										   ;
	jmp		__int_80_ret			;										   ;
				
__check_SYS_MEMMAP:
	cmp		eax, _SYS_MEMMAP
	jnz		__int_80_ret
	
									;	default:							   ;
__int_80_ret:						;		break:							   ;
	leave							; }										   ;
	iret							;										   ;
;------------------------------------------------------------------------------;

;------------------------------------------------------------------------------;
; _int_74_handler                                                              ;
;                                                                              ;
;   Brief: It handles the interrupt of the mouse (IRQ 12) by calling the       ;
;   mouse driver.															   ;
;------------------------------------------------------------------------------;
_int_74_handler:                    ;                                          ;
    push    ebp                     ; Building Stack Frame                     ;
	mov     ebp, esp                ;                                          ;
	and		esp, -16				;										   ;
	pusha                           ; 										   ;                
									;										   ;
	call    mouseDriver				;										   ;
	mov		al, 0x20				; Sending generic EOI to PIC 1 & 2		   ;
	out		0xA0, al                ;										   ;
	out		0x20, al				;										   ;
									;										   ;
	popa							;										   ;
	leave							;										   ;
	iret							;									       ;
;------------------------------------------------------------------------------;

;------------------------------------------------------------------------------;
; _int_09_handler															   ;
;																			   ;
;	Brief: 	This is the routine to manipulate the interrupt number 9. Then call;
;			to the															   ;
;			keyboard_driver that it's implemented in C.						   ;
; -----------------------------------------------------------------------------;
_int_09_handler:						; Building the stack Frame			   ;
        push    ds						;									   ;
        push    es                      ; 									   ;
        pusha                           ; 									   ;
        call    keyboard_driver         ; Call to the keyboard_driver (imple.C);
		mov		al,20h					; Send EOI							   ;
		out		20h,al					; Write the instruction in port 0x20   ;
		popa							; Leave the Stack Frame				   ;
		pop		es						;									   ;
		pop		ds						;									   ;
		iret							;									   ;
; ---------------------------------------------------------------------------- ;

SECTION .data

	_SYS_MEMMAP 	equ 2
	