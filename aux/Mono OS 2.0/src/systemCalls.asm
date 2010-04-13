GLOBAL sysWrite
GLOBAL sysRead

EXTERN _sys_writeController
;EXTERN _sys_write
;EXTERN _sys_read
EXTERN _sys_readController
EXTERN fdTable
EXTERN _debug 
EXTERN SIZE_OF_FDELEM
EXTERN SIZE_OF_FDELEM_UNIT
EXTERN IO_FLAG
EXTERN M_FLAG

SECTION .text

;-----------------------------o--------------------------------
; Function: sysWrite
; Description:
;		Writes on File Descriptor the buffer
;
; Input:
;		EBX = File Descriptor (fd)
;		ECX=  buffer [memory address]
;		EDX = size of buffer [bytes]
; Output:	
;		EAX=amount of bytes written
;		
; Programmer:
;		Bermudez, Perez De Rosso, Pizzagalli
;
; Version:	
;		22.05.09.22.29
;---------------------------------------------------------------

sysRead:
	push EBP
	mov BP, 0				; 0 is reading code
	;call sysRW
	;jmp sysRW
	push EDX
	push ECX
	push EBX
	call _sys_readController
	pop EBX
	pop ECX
	pop EDX
	pop EBP
	ret
	call _debug	
sysWrite:
	push EBP
	mov BP, 1				; 1 is writing code
	;call sysRW
	;jmp sysRW
	push EDX
	push ECX
	push EBX
	call _sys_writeController
;	call _sys_write
	pop EBX
	pop ECX
	pop EDX
	pop EBP
	ret
	call _debug
sysRW:
	
	pushAD		
	cmp EDX, 00h				; if the size of buffer to write is ...
	jbe endWithError			; ... < 1 --> end with error
	

	push EDX
	mov EAX, EBX				; calculates the amount of bytes to shift from the initial... 
	mov ESI, [DS:SIZE_OF_FDELEM]
	mul SI					; ...position of the table...
	mov ESI, EAX				; ...and stores it in the data offset pointer

	mov EDI, fdTable				; loads the initial position of the file description table 

	add EDI, ESI
	pop EDX
	push EDX
	push EDI
	add EDI, [DS:SIZE_OF_FDELEM_UNIT]
	mov EAX, [DS:EDI]		 	; compares the offset + the amount of bytes...
	add EAX, EDX				; ... to write with the...
	add EDI, [DS:SIZE_OF_FDELEM_UNIT]
	cmp EAX, [DS:EDI]			 ; ...EOF value
	jnb endWithErrorPop			; if Offset + amount of bytes - EOF > 0 --> out of range --> end with error

	;if we got to this point the offset is in range

	pop EDI
	mov EAX, [DS:EDI] 			; calculates the memory position where to ...
	add EDI, [DS:SIZE_OF_FDELEM_UNIT]
	add EAX, [DS:EDI]			; ...write the buffer (base + offset) 	


	add EDI, [DS:SIZE_OF_FDELEM_UNIT]
	add EDI, [DS:SIZE_OF_FDELEM_UNIT]
	mov EBX, [DS:EDI] ; Checks the flag so as to ... 
	cmp EBX, [DS:IO_FLAG]			; ...know wether the write should be to ...
	je io					; ...input/output...
	cmp EBX, [DS:M_FLAG]				; ...or...
	je memory				; ...memory
	jmp endWithError			; otherwise, the flag is invalid --> end with error


io:	
	pop EDX
ioDecide:
	push EAX
	push EDX

	cmp BP, 00h
	je ioRead
	jmp ioWrite	

ioInc:
	pop EDX
	inc ECX	
	pop EAX
	inc EAX
	dec EDX
	jnz ioDecide
	jmp end

ioWrite:
	mov DX, AX
	mov AL, [DS:ECX]
	out DX, AL 
	jmp ioInc

ioRead:
	mov DX, CX
	in AL, DX 
	mov [DS:ECX], AL
	jmp ioInc

memory:
	pop EDX
memDecide:
	cmp BP, 00h
	je memoryRead
	jmp memoryWrite	

memoryInc:
	inc ECX
	inc EAX
	dec EDX
	jnz memDecide
	jmp end

memoryWrite:
	mov BH, [DS:ECX]
	mov [DS:EAX], BH 
	jmp memoryInc

memoryRead:
	mov BH, [DS:EAX]
	mov [DS:ECX], BH 
	jmp memoryInc

end:
	popAD
	mov EAX, EDX
	ret

endWithErrorPop:
	pop EDI
endWithError:
	pop EDX
	popAD
	mov EAX, -1
	ret
