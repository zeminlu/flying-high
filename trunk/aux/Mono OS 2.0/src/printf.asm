SECTION .text
GLOBAL printf
EXTERN _debug
EXTERN sprintint
EXTERN sprintstr
EXTERN writeprintf

;-------------------------------o-------------------------------------
; Function: printf
; Description:		
;		Implementation of sprintf with variable args. 
;		It is the "primiteve" of printf, wich sends this string to video.
;		CAUTION!!: Only suports %d %c %s argument types.
;		WARNING!!: Only suports up to 10000 chars in the output string.
; 		OJOOOOOOOOOOO REVISARRRRRRR LIMITEEEEEEE SE CALCULA MAL!!!!!!!
; Input:		
;		EAX= guide string pointer
;		EBX= buffer  position
;		ECX= amount of chars copied
;		EDX= operand register
;		EBP= arguments pointer
; Output:
;		EAX=position of the new String.
; Programmer:
;		SSP
; Version:
;		24.05.09.04.02
;--------------------------------------------------------------------------


printf:
	push EBP
	mov EBP, ESP
	pushAD
	add EBP,8
	mov EAX,[DS:EBP]	;cargo el string de printf
	mov ECX, 0		;counter of chars written in buffer ( for no overflow making)
	mov EBX, buff		;internar buffer first possition
loop:
	mov DH,[DS:EAX]
	cmp DH,0		;if the end of the string is reached ...
	je end			;... jump to end
	cmp DH,'%'		;me fijo si puede ser un argumento a imprimir...
	je maybeanarg		;... salto
notanarg:
	mov DH,[DS:EAX]		;prepearing to wirte...
	mov [DS:EBX],DH		;writting
	inc ECX			;++(written character)
	mov EDX,lim
	cmp ECX,EDX		; if limit is reached...
	je end			;end function
	inc EAX			;go to next char in string
	inc EBX			;go to next plase to write in buffer
	jmp loop		;continue
maybeanarg:
	inc EAX			;looking for next char, 
	mov DH,[DS:EAX]
	cmp DH,'d'		;if it is d ---> integer...
	je integer		;... jump 
	cmp DH,'c'
	je char
	cmp DH,'s'		;if it is s---> string ....
	je string		;... jump
;	cmp byte [DS:EAX],'f'	;if it is f ---> float ...
;	je float		;... jump
	mov DH,'%'
	mov [DS:EBX],DH		;if it is not an argument,  write "%" and
	inc EBX			;increase buffer pointer
	inc ECX			;increase counter
	jmp notanarg		;return to the loop of not arguments
char:
	add EBP,4		;point the arg that will be used
	mov DH,[DS:EBP]		;... and print it to the...
	mov [DS:EBX],DH		;... buffer
	inc EAX			;move the guide string pointer
	inc ECX			;++(written character)
	inc EBX			;move foward the buffer pointer
	jmp loop		

integer:
	add EBP,4		;point the arg that will be used
	mov EDX,[DS:EBP]
	push EAX		;saves the actual position in the string guide
	push EDX		;push the the integer
	push EBX		;push the pointer to the buffer
	mov EDX,lim		;begins calculating limit
	sub EDX,ECX		;....
;	add EDX,lim		;finishs calculatin limit
	push EDX
	call sprintint
	add ECX,EAX		;adds to counter the number of chars copied in sprintint
	add EBX,EAX		;moves the buffer pointer, to sync with chars copied en sprintint
	pop EDX			;throw all the garbage
	pop EDX			;throw all the garbage
	pop EDX			;throw all the garbage
	pop EAX			;recover EAX
	inc EAX			;go to next char
	jmp loop

string:	
	add EBP,4		;point the arg that will be used
	mov EDX,[DS:EBP]	;load the pointer
	push EAX		;saves the actual position in the string guide
	push EDX		;push the pointer to the string
	push EBX		;push the pointer to the buffer
	mov EDX,lim		;begins calculating limit
	sub EDX,ECX		;....
;	add EDX,lim		;...finishs calculatin limit...
	push EDX		;... send it
	call sprintstr
	add ECX,EAX		;adds to counter the number of chars copied in sprintint
	add EBX,EAX		;moves the buffer pointer, to sync with chars copied en sprintint
	pop EDX			;throw all the garbage
	pop EDX			;IDEM
	pop EDX			;IDEM
	pop EAX			;recover EAX
	inc EAX			;go to next char
	jmp loop

end:
	inc EBX			;finish the string...
	mov byte [DS:EBX],0	;...with a null
	mov EAX,buff
	push EAX
	call writeprintf
	pop EAX
	popAD
	pop EBP

	ret

	
SECTION .data
buff	db	10000
lim	equ	10000
