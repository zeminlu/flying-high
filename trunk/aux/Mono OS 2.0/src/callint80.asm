;
;	File: callint80.asm
;
;

GLOBAL callInt80
GLOBAL callInt802
EXTERN _debug
;
; Function: void * callInt80( int eax, void * ebx, void * ecx, void * edx )
;

callInt80:
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
;
; Function: void * callInt80( int eax, void * ebx, void * ecx, void * edx, void *edi, void *esi)
;

callInt802:
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
	add EBP,4		;point next arg ( edi)
	mov EDI,[SS:EBP]	;load it
	add EBP,4		;point next arg ( esi)
	mov ESI,[SS:EBP]	;load it
	int 80h
	pop EBP
	ret
