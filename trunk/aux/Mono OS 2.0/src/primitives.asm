GLOBAL write
GLOBAL read 

GLOBAL ID_WRITE
GLOBAL ID_READ

EXTERN sysWrite
EXTERN _debug 

SECTION .text
;-------------------------------o-------------------------
; Function: ssize_t write(int fd, const void *buf, size_t count) 
;
; Description:
;		Writes on File Descriptor the buffer
;
; Input:
;		fd = File Descriptor (fd)
;		buf=  buffer [memory address]
;		count= size of buffer [bytes]
; Output:	
;		amount of bytes written
;		
; Programmer:
;		Bermudez, Perez De Rosso, Pizzagalli
;
; Version:	
;		23.05.09.15.29
;---------------------------------------------------------------


write:
	mov EAX, ID_WRITE
	jmp primRW
;---------------------------------o-------------------------------------
; Function: ssize_t read(int fd, const void *buf, size_t count) 
;
; Description:
;		Reads on File Descriptor the buffer
;
; Input:
;		fd = File Descriptor (fd)
;		buf=  buffer [memory address]
;		count= size of buffer [bytes]
; Output:	
;		amount of bytes read 
;		
; Programmer:
;		Bermudez, Perez De Rosso, Pizzagalli
;
; Version:	
;		23.05.09.15.29
;-----------------------------------------------------------------------
read:
	mov EAX, ID_READ
	jmp primRW

primRW:
	push EBP
	mov EBP, ESP
	mov EBX, [SS: EBP + 8 ] 	; levanta int
	mov ECX, [SS: EBP + 12 ]	; levanta puntero
	mov EDX, [SS: EBP + 16 ]	; levanta int
	push EBP
	int 80h
	pop EBP
	mov ESP, EBP
	pop EBP
	;call _debug
	ret

ID_WRITE equ 4
ID_READ equ 3
