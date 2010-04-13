EXTERN sysWrite
EXTERN sysOpen
EXTERN sysClose
EXTERN sysRmTag
EXTERN sysMkTag
EXTERN sysRmFile
EXTERN sysReadTag
EXTERN sysRead
EXTERN sysOpenTag
EXTERN sysGetFileInfo
EXTERN sysExit
EXTERN sysCreateProcess
EXTERN ID_WRITE
EXTERN ID_READ
EXTERN _debug
EXTERN int16
EXTERN mouseHandler
EXTERN activityDetected
EXTERN panic

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
EXTERN multitasker
EXTERN tickCounter

EXTERN enterKernelSpace
EXTERN exitKernelSpace

EXTERN sysSetTty
EXTERN sysGetTty
EXTERN sysGetpid
EXTERN sysDup
EXTERN sysDup2
EXTERN sysGetlevel
EXTERN sysSetlevel
EXTERN sysGetpriority
EXTERN sysSetpriority
EXTERN sysTime
EXTERN sysWaitpid
EXTERN sysDup2
EXTERN sysSetpriority
EXTERN sysKill

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

GLOBAL mouse_int_served

GLOBAL _int_08_hand
GLOBAL _int_80_hand
GLOBAL _int_16_hand
GLOBAL _int_09_hand
GLOBAL _int_20_hand
GLOBAL _int_74_hand

GLOBAL mtaskRetAddr
GLOBAL multitasker2

[BITS 32]

SECTION .text
; Macro
; This macro checks if the value in AL corresponds to a down code or 
; up code of a modifier. If the value corresponds to a down code then 
; it does the operation (modifiers v downMask). If the value corresponds
; to an up code the it does the operation (modifiers & upMask). In both 
; cases it jumps to modEnd. Also, if the value in AL doesn't match the
; modifiers up or down code it jumps to modNext 
;
;checkMod	MACRO modNext, modEnd, downCode, upCode, downMask, upMask
;	local modDown, modUp
;	cmp AL, downCode	
;	je modDown
;	cmp AL, upCode 
;	je modUp
;	jmp modNext
;modDown:
;	or modifiers, downMask 
;	jmp modEnd
;modUp:
;	and modifiers, upMask 
;	jmp modEnd
;	endm

;checkLock 	MACRO modNext, modEnd, upCode, upMask
;	local modUp
;	cmp AL, upCode
;	je modUp
;	jmp modNext
;modUp:
;	and modifiers, upMask 
;	jmp modEnd
;	endm

multitasker2:
	push ebp
	mov	ebp, esp

	int 20h

	leave
	ret

;-----------------------------------------------------------------------------;
; Function: _devide_e_hand													  ;
; Description:																  ;
;		Handles the divide by zero exception.								  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_divide_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call divideExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _debug_e_hand													  ;
; Description:																  ;
;		Handles the debug exception.										  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_debug_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call debugExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _nmi_e_hand														  ;
; Description:																  ;
;		Handles the non maskable exception.									  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_nmi_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call NMIExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _break_point_e_hand												  ;
; Description:																  ;
;		Handles the break point exception.									  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_break_point_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call breakPointExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _overflow_e_hand												  ;
; Description:																  ;
;		Handles the overflow exception.										  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_overflow_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call overflowExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _bounds_check_e_hand											  ;
; Description:																  ;
;		Handles the bounds check exception.									  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_bounds_check_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call boundsCheckExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _invalid_opcode_e_hand											  ;
; Description:																  ;
;		Handles the invalid opcode exception.								  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_invalid_opcode_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call invalidOpcodeExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _copro_unavailable_e_hand										  ;
; Description:																  ;
;		Handles the coprocessor unavailable exception.						  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_copro_unavailable_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call coprocessorUnavailableExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _double_fault_e_hand											  ;
; Description:																  ;
;		Handles the double fault exception.									  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_double_fault_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call doubleFaultExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _invalid_tss_e_hand			 									  ;
; Description:																  ;
;		Handles the invalid TSS exception.									  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_invalid_tss_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call invalidTSSExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _segment_not_present_e_hand										  ;
; Description:																  ;
;		Handles the segment not present exception.							  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_segment_not_present_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call segmentNotPresentExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _stack_e_hand													  ;
; Description:																  ;
;		Handles the stack exception.										  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_stack_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call stackExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _general_protection_e_hand										  ;
; Description:																  ;
;		Handles the general protection exception.							  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_general_protection_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call generalProtectionExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _page_fault_e_hand												  ;
; Description:																  ;
;		Handles the page fault exception.									  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_page_fault_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call pageFaultExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret

;-----------------------------------------------------------------------------;
; Function: _coprocessor_e_hand												  ;
; Description:																  ;
;		Handles the coprocessor exception.									  ;
; 																			  ;
; Programmer: Guido Marucci Blas											  ;
;-----------------------------------------------------------------------------;
_coprocessor_e_hand:
	cli
	pushad
	call enterKernelSpace
	
	call coprocessorExceptionHandler
	
	push 0x01
	call exitKernelSpace
	add esp, 0x04

	popad
	iret
;-----------------------------------------------------------------------------;
; Function: _int_20_hand_													  ;
; Description:																  ;
;		Calls the time tick interrupt without counting the ticks			  ;
;																			  ;
; Programer: Guido Marucci Blas												  ;
;-----------------------------------------------------------------------------;
_int_20_hand:
	mov byte [countTick], 00h ;There is no iret because the _int_08_hand MUST BE CALLED	

;-----------------------------------------------------------------------------;
; Function: _int_08_hand_													  ;
; Description:																  ;
;		Handles The timer tick interrupt and calls the multitasker			  ;
;																			  ;
; Programer: Guido Marucci Blas												  ;
;-----------------------------------------------------------------------------;
_int_08_hand:
	cli
	
	cmp byte [countTick], 01h
	jnz	callMultitasker 
	inc	byte [tickCounter];

;slowDownMultiTasker:		;DEBUG
;	push edi				;DEBUG
;	mov edi, [tickCounter]	;DEBUG
;	and edi, 003h			;DEBUG
;	pop	edi					;DEBUG
;	jz callMultitasker		;DEBUG
;	jmp mtaskRetAddr		;DEBUG

callMultitasker:			
	mov byte [countTick], 01h
	pushad
	call enterKernelSpace
	
	call multitasker

mtaskRetAddr:
	mov al, 20h
	out 20h, al

	push dword 0x0
	call exitKernelSpace
	add esp, 0x04
	popad
	iret


;-----------------------------o---------------------------------
; Function: _int_80_hand
; Description:
;		Handles the 80h interruption when generated. 
;		Decides wether the interruption was generated 
;		with intentions to "write to" or "read from" 
;		memory.
; Input:
;		EAX (Intention descriptor [ID_intention])
;			=4 if a write is intended
;			=3 if a read is intended
;		EBX = File Descriptor (fd)
;		ECX=  buffer [memory address]
;		EDX = size of buffer [bytes]
; Output:	
;		EAX=amount of bytes read or written
;		
; Programmer:
;		Bermudez, Perez De Rosso, Pizzagalli
;
; Version:	
;		22.05.09.15.51
;---------------------------------------------------------------
_int_80_hand:
	cli
	pushad
	call enterKernelSpace
	popad

	cmp EAX, ID_WRITE
	je write_
	cmp EAX, ID_READ
	je read_
	cmp EAX, ID_OPEN
	je open
	cmp EAX, ID_CLOSE
	je close
	cmp EAX, ID_MKTAG
	je mktag
	cmp EAX, ID_RMFILE
	je rmfile
	cmp EAX, ID_READTAG
	je readtag
	cmp EAX, ID_RMTAG
	je rmtag
	cmp EAX, ID_OPENTAG
	je opentag
	cmp EAX, ID_FILEINFO
	je getfileinfo
	cmp EAX, ID_EXIT
	je exit
	cmp EAX, ID_FORK
	je create
	cmp EAX, ID_GET_TTY
	je getTty
	cmp EAX, ID_SET_TTY
	je setTty
	cmp EAX, ID_GET_PID
	je getPid
	cmp EAX, ID_WAIT_PID
	je waitPid
	cmp EAX, ID_DUP_2
	je dup2
	cmp EAX, ID_KILL
	je kill
	cmp EAX, ID_SET_PRIO
	je setPriority
	mov EAX, -1	; if not a valid intention returns the error code

endInt80:
	pushad
	push dword 0x01
	call exitKernelSpace
	add esp, 0x04
	popad
	iret
	
open:
	push EDX
	push ECX
	push EBX
	call sysOpen
	pop EBX
	pop ECX
	pop EDX
	;TODO cambiarlo
	;iret
	jmp endInt80
close:
	push EBX
	call sysClose
	pop EBX
	;TODO cambiarlo
	;iret
	jmp endInt80
mktag:
	push EBX
	call sysMkTag
	pop EBX
	;TODO cambiarlo
	;iret
	jmp endInt80
rmfile:
	push EBX
	call sysRmFile
	pop EBX
	;TODO cambiarlo
	;iret
	jmp endInt80
readtag:
	push EBX
	call sysReadTag
	pop EBX
	;TODO cambiarlo
	;iret
	jmp endInt80
rmtag:
	push EBX
	call sysRmTag
	pop EBX
	;TODO cambiarlo
	;iret
	jmp endInt80
opentag:
	push EBX
	call sysOpenTag
	pop EBX
	;TODO cambiarlo
	;iret
	jmp endInt80
getfileinfo:
	push EBX
	call sysGetFileInfo
	pop EBX
	;TODO cambiarlo
	;iret
	jmp endInt80
write_:
	call sysWrite
	;call _debug
	jmp endInt80
read_:
	call sysRead
	jmp endInt80
exit:
	push EBX
	call sysExit
	pop EBX
	jmp endInt80
create:
	push ESI
	push EDI
	push EDX
	push ECX
	push EBX
	call sysCreateProcess
	pop EBX
	pop ECX
	pop EDX
	pop EDI
	pop ESI
	jmp endInt80
waitPid:
	push EDX
	push ECX
	push EBX
	call sysWaitpid
	pop EBX
	pop ECX
	pop EDX
	jmp endInt80
getTty:
	push EBX
	call sysGetTty
	pop EBX
	jmp endInt80
setTty:
	push ECX
	push EBX
	call sysSetTty
	pop EBX
	pop ECX
	jmp endInt80
getPid:
	call sysGetpid
	jmp endInt80
dup2:
	push ECX
	push EBX
	call sysDup2
	pop EBX
	pop ECX
	jmp endInt80
kill:
	push ECX
	push EBX
	call sysKill
	pop EBX
	pop ECX
	jmp endInt80
setPriority:
	push ECX
	push EBX
	call sysSetpriority
	pop EBX
	pop ECX
	jmp endInt80

;-----------------------------o---------------------------------
; Function: _int_16_hand
; Description:
;		Handles the 16h interruption when generated. 
;		Is the link between the keyboard and the 
;		applications.
; Input:
;		AH	-->	An Ascii code
; Programmer:
;		Bermudez, Perez De Rosso, Pizzagalli
;
; Version:	
;		24.05.09.01.52
;---------------------------------------------------------------

_int_16_hand:
	cli
	mov AL, AH
	and EAX, 00FFh
	push EAX
	call int16
	pop EAX
	iret


;---------------------------o-----------------------------------------
; Function: _int_09_hand
;
; Description:
;		Handles the keyboard interruption when generated.
;		Is the interface between the keyboard hardware 
;		and the int 16h.
;		Builds up the 16 bit value where:
;			low part: scan code
;			high part: ascii code
;		and stores it in the circular buffer
;
; Input:
;		-
; Output:	
;		-
;--------------------------------------------------------------------

_int_09_hand:
	cli
	pushAD	
	call activityDetected
	mov DX, 64h	; checks wether the data is...
	in AL, DX	; ...is available for reading...
	and AL, 1h	; ...(this happens if bit 0 of the port 64h...
	jz _int_09_hand ; ... has a 1)
	
	mov DX, 60h	; gets in AL the scan code of the ...
	in AL, DX	; ...keyboard key that interrupted


	mov ESI, superTable ; loads the table with the ascii codes
	push EAX
	and EAX, 00FFh		; ESI will point to the place in the table that...
	dec EAX
	mov BX, 09h		; ...corresponds to the scan code obtained...
	mul BX	 		; ...(superTable + (scanCode-1) * 9 )
	add ESI, EAX		
	pop EAX


	;Note: In order to apply the key modifiers the following 
	; structure is used
	;	modifiers
	;  |   |   |   |   |   |   |   |   |
	;    7   6   5   4   3   2   1   0
	;
	; b_{0}: Right shift key pressed
	; b_{1}: Left shift key pressed
	; b_{2}: Ctrl key pressed
	; b_{3}: Alt key pressed
	; b_{4}: Scroll Lock on
	; b_{5}: Num Lock on
	; b_{6}: Caps Lock on
	; b_{7}: Insert on

	;checks wether the key is a modifier or not

	; Left shift case

checkLShift:
	cmp AL, 2Ah 
	je modDown1
	cmp AL, 0AAh 
	je modUp1
	jmp checkRShift 
modDown1:
	mov AH, [modifiers]
	or AH, 2h
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd
modUp1:
	mov AH, [modifiers]
	and AH, 0FDh 
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd


checkRShift:
	cmp AL, 36h 
	je modDown2
	cmp AL, 0B6h 
	je modUp2
	jmp checkControl 
modDown2:
	mov AH, [modifiers]
	or AH, 1h
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd
modUp2:
	mov AH, [modifiers]
	and AH, 0FEh 
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd

checkControl:
	cmp AL, 1Dh 
	je modDown3
	cmp AL, 9Dh 
	je modUp3
	jmp checkAlt
modDown3:
	mov AH, [modifiers]
	or AH, 4h
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd
modUp3:
	mov AH, [modifiers]
	and AH, 0FBh 
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd


checkAlt:
	cmp AL, 38h 
	je modDown4
	cmp AL, 0B8h 
	je modUp4
	jmp checkScrollLock
modDown4:
	mov AH, [modifiers]
	or AH, 8h
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd
modUp4:
	mov AH, [modifiers]
	and AH, 0F7h 
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd


checkScrollLock:
	cmp AL, 46h
	je modUp5
	jmp checkNumLock 
modUp5:
	mov AH, [modifiers]
	and AH, 10h 
	mov AH, [modifiers]
	jz notActive5
	and AH, 0EFh
	jmp cont5
notActive5:
	or AH, 10h
cont5:
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd


checkNumLock:
	cmp AL, 45h
	je modUp6
	jmp checkCapsLock
modUp6:
	mov AH, [modifiers]
	and AH, 20h 
	mov AH, [modifiers]
	jz notActive6
	and AH, 0DFh
	jmp cont6
notActive6:
	or AH, 20h
cont6:
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd



checkCapsLock:
	cmp AL, 3Ah
	je modUp7
	jmp continue 
modUp7:
	mov AH, [modifiers]
	and AH, 40h
	mov AH, [modifiers]
	jz notActive7
	and AH, 0BFh 
	jmp cont7
notActive7:
	or AH, 40h
cont7:
	mov EDI, modifiers
	mov [EDI], AH 
	jmp modEnd


; if we got to continue then the scan code doesn't correspond to a key
; modifier --> it is a letter
continue:

; Based on the modifiers We have to choose the correct position of the table
; there are 4 important cases: 
;
; case i: shift: yes caps lock: no
; case ii: shift: no caps lock: yes 
; case iv: shift: no caps lock: no
; case v: shift: yes caps lock: yes

	
	; First i must check if the ascii of the non-modifier key corresponds to a make code or to a break code
	; if it is a break code --> send the corresponding code and 00 on ascii
	push EAX
	mov EAX, endOfTable
	cmp EAX, ESI		; does the operation (memory position of the end of table - corresponding position due to the code obtained)
	pop EAX
	jbe noModBreak

	mov BP, 00h
	mov AH, [modifiers]	; sets to 0 all the modifiers but right shift, ...
	push AX
	and AH, 03h		; ... and left shift
	jz shiftOff
	inc BP
shiftOff:
	pop AX
	and AH, 40h		; sets to 0 all the modifiers but the caps lock
	jz capsLockOff
	add BP, 5h
capsLockOff:
	mov AL, [ESI]
	and EBP, 000FFh
	add ESI, EBP
	inc ESI
	mov AH, [ESI]
	int 16h
	call prepKB
	popAD
	iret
modEnd:
	mov AL, [ESI]
	mov AH, 00h 
	int 16h
	call prepKB
	popAD
	iret

noModBreak:
	mov AH, 00h
	int 16h	
	call prepKB
	popAD
	iret



prepKB:
	mov AL, 20h
	out 20h, AL
	ret
prepKB1:
	in AL, 64h		; checks wether the keyboard is ...
	and AL, 01h			; ...available for reading a ....
	jnz prepKB1		; ...command


	; the kb is ready to read a command 
	mov AL, 20h
	out 64h, AL		; this request the keyboard to write the keyboard...
				; ...microcontroller command byte on port 60h
waitData:
	in AL, 64h		; waits for the data to be able to..
	and AL, 01h			; ...be read from the port...
	jz waitData		; ...60h
	
	in AL, 60h		; sets the Keyboard Microcontroller Command Byte so as...
	or AL, 03h		; ...to re enable the keyboard & mouse interruptions
prepKB2:
	in AL, 64h		; checks wether the keyboard is ...
	and AL, 01h			; ...available for reading a ....
	jnz prepKB2		; ...command
	
	out 60h, AL
	ret


;-----------------------------o---------------------------------
;mouse_int_served:
;		Description:
;			tels the mouse, that the interruption has been
;			atended(by sending the 2 pics EOI
;
;---------------------------------------------------------------
mouse_int_served:
	push EAX
	mov  AL,20h		
	out  0xA0,AL	
	out  20h,AL
	pop  EAX
	ret
;-----------------------------o---------------------------------
; Function: _int_74_hand
; Description:
;		Handles the mouse(74h) interruption when generated. 
;		Decides wether the interruption was generated 
;		by the mouse, and updates de position of it
;
; FLAG BYTE DESCRIPTION:
;  |   |   |   |   |   |   |   |   |
;    7   6   5   4   3   2   1   0
;
;	byte{0} = tels wether right botton is pressed
;	byte{1} = tels wether left botton is pressed
; 	byte{2} = tels wether middle botton is pressed
;	byte{3} = maintain and verify packet alignment (always 1)
;	byte{4} = if on --> deltaX is negative
;	byte{5} = if on --> deltaY is negative
;	byte{6} = if on --> X overfload
;	byte{7} = if on --> Y overfload
; Programmer:
;		ssp
; Version:	
;		24.05.09.18.41
;---------------------------------------------------------------

_int_74_hand:
	push EAX
	call activityDetected
	in  AL,64h
	and AL,21h
	cmp AL,21h
	jne end_mouse
	call mouseHandler
end_mouse:
	pop EAX
	iret





	
	
	

; DEVOLVER EN AX EL PAR ORDENADO AH = ascii AL = scan code

SECTION .data
countTick db 01h

	ID_EXIT			equ 1
	ID_FORK			equ 2
	ID_OPEN 		equ 5
	ID_CLOSE		equ 6
	ID_MKTAG		equ	11
	ID_RMFILE		equ	9
	ID_READTAG		equ	10
	ID_RMTAG		equ 12
	ID_OPENTAG		equ 13
	ID_FILEINFO		equ	14

	ID_SET_TTY		equ 30
	ID_GET_TTY		equ 31
	ID_GET_PID		equ 32
	ID_SET_PID      equ 33
	ID_GET_PRIO     equ 34
	ID_SET_PRIO     equ 35
	ID_GET_LEVEL    equ 36
	ID_SET_LEVEL    equ 37
	ID_DUP			equ 38
	ID_DUP_2		equ 39
	ID_WAIT_PID		equ 40
	ID_KILL			equ 41

modifiers db 00h
; Scan Code  ascii    +shift   +ctrl   +Alt   +Num     +Caps +ShiftCaps +shiftNum
superTable db	01h,	1Bh,	1Bh,	1Bh,	00h,	1Bh,	1Bh,	1Bh,	1Bh
db	02h,	31h,	21h,	00h,	00h,	31h,	31h,	21h, 	31h
db	03h,	32h,	40h,	00h,	00h,	32h,	32h,	40h,	32h
db	04h,	33h,	23h,	00h,	00h,	33h,	33h,	23h,	33h
db	05h,	34h,	24h,	00h,	00h,	34h,	34h,	24h,	34h
db	06h,	35h,	25h,	00h,	00h,	35h,	35h,	25h,	35h
db	07h,	36h,	5Eh,	1Eh,	00h,	36h,	36h,	5Eh,	36h
db	08h,	37h,	26h,	00h,	00h,	37h,	37h,	26h,	37h
db	09h,	38h,	2Ah,	00h,	00h,	38h,	38h,	2Ah,	38h
db	0Ah,	39h,	28h,	00h,	00h,	39h,	39h,	28h,	39h
db	0Bh,	30h,	29h,	00h,	00h,	30h,	30h,	29h,	30h
db	0Ch,	2Dh,	5Fh,	1Fh,	00h,	2Dh,	2Dh,	5Fh,	5Fh
db	0Dh,	3Dh,	2Bh,	00h,	00h,	3Dh,	3Dh,	2Bh,	2Bh
db	0Eh,	08h,	08h,	7Fh,	00h,	08h,	08h,	08h,	08h
db	0Fh,	09h,	00h,	00h,	00h,	09h,	09h,	00h,	00h
db	10h,	71h,	51h,	11h,	00h,	71h,	51h,	71h,	51h
db	11h,	77h,	57h,	17h,	00h,	77h,	57h,	77h,	57h
db	12h,	65h,	45h,	05h,	00h,	65h,	45h,	65h,	45h
db	13h,	72h,	52h,	12h,	00h,	72h,	52h,	72h,	52h
db	14h,	74h,	54h,	14h,	00h,	74h,	54h,	74h,	54h
db	15h,	79h,	59h,	19h,	00h,	79h,	59h,	79h,	59h
db	16h,	75h,	55h,	15h,	00h,	75h,	55h,	75h,	55h
db	17h,	69h,	49h,	09h,	00h,	69h,	49h,	69h,	49h
db	18h,	6Fh,	4Fh,	0Fh,	00h,	6Fh,	4Fh,	6Fh,	4Fh
db	19h,	70h,	50h,	10h,	00h,	70h,	50h,	70h,	50h
db	1Ah,	5Bh,	7Bh,	1Bh,	00h,	5Bh,	5Bh,	7Bh,	7Bh
db	1Bh,	5Dh,	7Dh,	1Dh,	00h,	5Dh,	5Dh,	7Dh,	7Dh
db	1Ch,	0Dh,	0Dh,	0Ah,	00h,	0Dh,	0Dh,	0Ah,	0Ah
db	1Dh,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	1Eh,	61h,	41h,	01h,	00h,	61h,	41h,	61h,	41h
db	1Fh,	73h,	53h,	13h,	00h,	73h,	53h,	73h,	53h
db	20h,	64h,	44h,	04h,	00h,	64h,	44h,	64h,	44h
db	21h,	66h,	46h,	06h,	00h,	66h,	46h,	66h,	46h
db	22h,	67h,	47h,	07h,	00h,	67h,	47h,	67h,	47h
db	23h,	68h,	48h,	08h,	00h,	68h,	48h,	68h,	48h
db	24h,	6Ah,	4Ah,	0Ah,	00h,	6Ah,	4Ah,	6Ah,	4Ah
db	25h,	6Bh,	4Bh,	0Bh,	00h,	6Bh,	4Bh,	6Bh,	4Bh
db	26h,	6Ch,	4Ch,	0Ch,	00h,	6Ch,	4Ch,	6Ch,	4Ch
db	27h,	3Bh,	3Ah,	00h,	00h,	3Bh,	3Bh,	3Ah,	3Ah
db	28h,	27h,	22h,	00h,	00h,	27h,	27h,	22h,	22h
db	29h,	60h,	7Eh,	00h,	00h,	60h,	60h,	7Eh,	7Eh
db	2Ah,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	2Bh,	5Ch, 	7Ch,	1Ch,	00h,	5Ch,	5Ch,	7Ch,	7Ch
db	2Ch,	7Ah, 	5Ah,	1Ah,	00h,	7Ah,	5Ah,	7Ah,	5Ah
db	2Dh,	78h,	58h,	18h,	00h,	78h,	58h,	78h,	58h
db	2Eh,	63h,	43h,	03h,	00h,	63h,	43h,	63h,	43h
db	2Fh,	76h,	56h,	16h,	00h,	76h,	56h,	76h,	56h
db	30h,	62h,	42h,	02h,	00h,	62h,	42h,	62h,	42h
db	31h,	6Eh,	4Eh,	0Eh,	00h,	6Eh,	4Eh,	6Eh,	4Eh
db	32h,	6Dh,	4Dh,	0Dh,	00h,	6Dh,	4Dh,	6Dh,	4Dh
db	33h,	2Ch,	3Ch,	00h,	00h,	2Ch,	2Ch,	3Ch,	3Ch
db	34h,	2Eh,	3Eh,	00h,	00h,	2Eh,	2Eh,	3Eh,	3Eh
db	35h,	2Fh,	3Fh,	00h,	00h,	2Fh,	2Fh,	3Fh,	3Fh
db	36h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	37h,	2Ah,	00h,	10h,	00h,	2Ah,	2Ah,	00h,	00h
db	38h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	39h,	20h,	20h,	20h,	00h,	20h,	20h,	20h,	20h
db	3Ah,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	3Bh,	80h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	3Ch,	81h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	3Dh,	82h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	3Eh,	83h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	3Fh,	84h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	40h,	85h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	41h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	42h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	43h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	44h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	45h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	46h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	47h,	00h,	37h,	00h,	00h,	37h,	00h,	37h,	00h
db	48h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	49h,	00h,	39h,	00h,	00h,	39h,	00h,	39h,	00h
db	4Ah,	2Dh,	2Dh,	00h,	00h,	2Dh,	2Dh,	2Dh,	2Dh
db	4Bh,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	4Ch,	00h,	35h,	00h,	00h,	35h,	00h,	35h,	00h
db	4Dh,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	4Eh,	2Bh,	2Bh,	00h,	00h,	2Bh,	2Bh,	2Bh,	2Bh
db	4Fh,	00h,	31h,	00h,	00h,	31h,	00h,	31h,	00h
db	50h,	00h,	00h,	00h,	00h,	00h,	00h,	00h,	00h
db	51h,	00h,	33h,	00h,	00h,	33h,	00h,	33h,	00h
db	52h,	00h,	30h,	00h,	00h,	30h,	00h,	30h,	00h
db	53h,	00h,	2Eh,	00h,	00h,	2Eh,	00h,	2Eh,	00h 
endOfTable db 00h
