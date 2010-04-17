;
;	File: int80.asm
;

GLOBAL int80

;
; Function: void * int80( int eax, void * ebx, void * ecx, void * edx )
;

int80:
	push EBP		;stack frame
	mov EBP,ESP
	add EBP,8		;point the first arg ( eax)
	mov EAX,[SS:EBP]	;load it
	add EBP,4		;point next arg ( ebx)
	mov EBX,[SS:EBP]	;load it
	add EBP,4		;point next arg ( ecx)
	mov ECX,[SS:EBP]	;load it
	add EBP,4		;point next arg ( edx)
	mov EDX,[SS:EBP]	;load it
	int 80h
	pop EBP
	ret
