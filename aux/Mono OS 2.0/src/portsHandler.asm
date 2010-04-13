GLOBAL outb
GLOBAL inb
SECTION .text

;-----------------------------o---------------------------------
; Function: outb
; Description:
;		Writes to port
;		 
; Input:	
;		in stack:
;		port
;		char
; Output:	
;		
; Programmer:
;		Lucas Pizzagalli
; Registers:
;		BL:port
;		AL:char to be written
; Version:	
;		24.05.09.18.41
;---------------------------------------------------------------

outb:
	push EBP		;stack frame
	mov EBP,ESP
	pushAD			
	add EBP,8		;point the first arg ( port)
	mov EDX,[DS:EBP]	;load it
	add EBP,4		;point next arg ( byte to write)
	mov AL,[DS:EBP]		;load it
	out DX,AL		;make the out
	popAD			;restoring
	pop EBP
	ret

;-----------------------------o---------------------------------
; Function: inb
; Description:
;		Writes to port
;		 
; Input:	
;		in stack:
;		port
; Output:	
;		EAX: byte readed	
; Programmer:
;		Lucas Pizzagalli
; Registers:
;		BL:port
;		AL:char to be read
; Version:	
;		24.05.09.18.41
;---------------------------------------------------------------

inb:
	push EBP		;stack frame
	mov EBP,ESP		
	push EDX
	add EBP,8		;point arg (port)
	mov EDX,[DS:EBP]	;load it
	in AL,DX		;make the in port
	pop EDX			;resoring
	pop EBP
	and EAX,0xFF		;destroy al most significant bits(I want a char)
	ret


