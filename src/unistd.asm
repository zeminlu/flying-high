;-----------------------------------------------------------------------------;
;	File: unistd.asm														  ;
;																			  ;
;		Brief: This file implements the system call defined in system.h		  ;
;																			  ;
;		Author: Guido Marucci Blas, Nicolas Purita & Luciano Zemin			  ;
;-----------------------------------------------------------------------------;
section .text

global write
global read

global fwrite
global fread

%include "sys.inc"

kernel:
	int 	0x80
	ret

;-----------------------------------------------------------------------------;
; write																		  ;
;																			  ;
;	Brief: Implements the following C function								  ;
; 	ssize_t write ( int fileds, const void * buffer, size_t count )			  ;
;																			  ;
;-----------------------------------------------------------------------------;
write:							;											  ;		
	push	ebp					; Building stack frame						  ;
	mov		ebp, esp			;											  ;
	and		esp, -16			; Aligning segment to 16 bytes				  ;
	push	ebx					; Saving caller's registers					  ;
	push	ecx					;											  ;
	push	edx					;											  ;
								;											  ;
								; Retrieving Parameters						  ;
	mov		edx, [ss:ebp + 16]	; Third: bytes to be written				  ;
	mov		ecx, [ss:ebp + 12]	; Second: pointer to buffer					  ;
	mov		ebx, [ss:ebp + 8]	; First: file descriptor					  ;
	mov		eax, _SYS_WRITE		; Calling write system call					  ;
	call	kernel				; 											  ;
								;											  ;
	pop		edx					; Restoring caller's registers				  ;
	pop		ecx					;											  ;
	pop		ebx					;											  ;
								;											  ;
	leave						;											  ;
	ret							;											  ;
;-----------------------------------------------------------------------------;

;-----------------------------------------------------------------------------;
; read																		  ;
;																			  ;														
;	Brief: Implements the following C function								  ;
; 	ssize_t read ( int fileds, void * buffer, size_t count )				  ;
;																			  ;
;-----------------------------------------------------------------------------;
read:							;											  ;						
	push	ebp					; Building stack frame						  ;
	mov		ebp, esp			;											  ;
	and		esp, -16			; Aligning segment to 16 bytes				  ;
	push	ebx					; Saving caller's registers					  ;
	push	ecx					;											  ;
	push	edx					;											  ;
								;											  ;
								; Retrieving Parameters						  ;
	mov		edx, [ss:ebp + 16]	; Third: bytes to be read					  ;
	mov		ecx, [ss:ebp + 12]	; Second: pointer to buffer					  ;
	mov		ebx, [ss:ebp + 8]	; First: file descriptor					  ;
	mov		eax, _SYS_READ		; Calling read system call					  ;
	call	kernel				; 											  ;
								;											  ;
	pop		edx					; Restoring caller's registers				  ;
	pop		ecx					;											  ;
	pop		ebx					;											  ;
								;											  ;
	leave						;											  ;
	ret							;											  ;
;-----------------------------------------------------------------------------;

;-----------------------------------------------------------------------------;
; write																		  ;
;																			  ;
;	Brief: Implements the following C function								  ;
; 	ssize_t write ( int fileds, const void * buffer, size_t count )			  ;
;																			  ;
;-----------------------------------------------------------------------------;
fwrite:							;											  ;		
	push	ebp					; Building stack frame						  ;
	mov		ebp, esp			;											  ;
	and		esp, -16			; Aligning segment to 16 bytes				  ;
	push	ebx					; Saving caller's registers					  ;
	push	ecx					;											  ;
	push	edx					;											  ;
								;											  ;
								; Retrieving Parameters						  ;
	mov		edx, [ss:ebp + 16]	; Third: bytes to be written				  ;
	mov		ecx, [ss:ebp + 12]	; Second: pointer to buffer					  ;
	mov		ebx, [ss:ebp + 8]	; First: file descriptor					  ;
	mov		eax, _SYS_FWRITE	; Calling write system call					  ;
	call	kernel				; 											  ;
								;											  ;
	pop		edx					; Restoring caller's registers				  ;
	pop		ecx					;											  ;
	pop		ebx					;											  ;
								;											  ;
	leave						;											  ;
	ret							;											  ;
;-----------------------------------------------------------------------------;

;-----------------------------------------------------------------------------;
; read																		  ;
;																			  ;														
;	Brief: Implements the following C function								  ;
; 	ssize_t read ( int fileds, void * buffer, size_t count )				  ;
;																			  ;
;-----------------------------------------------------------------------------;
fread:							;											  ;						
	push	ebp					; Building stack frame						  ;
	mov		ebp, esp			;											  ;
	and		esp, -16			; Aligning segment to 16 bytes				  ;
	push	ebx					; Saving caller's registers					  ;
	push	ecx					;											  ;
	push	edx					;											  ;
								;											  ;
								; Retrieving Parameters						  ;
	mov		edx, [ss:ebp + 16]	; Third: bytes to be read					  ;
	mov		ecx, [ss:ebp + 12]	; Second: pointer to buffer					  ;
	mov		ebx, [ss:ebp + 8]	; First: file descriptor					  ;
	mov		eax, _SYS_FREAD		; Calling read system call					  ;
	call	kernel				; 											  ;
								;											  ;
	pop		edx					; Restoring caller's registers				  ;
	pop		ecx					;											  ;
	pop		ebx					;											  ;
								;											  ;
	leave						;											  ;
	ret							;											  ;
;-----------------------------------------------------------------------------;
