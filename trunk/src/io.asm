;io.asm
global inportb
global outportb

section .text

inportb:
	push 	ebp
	mov		ebp, esp
	mov		dx, [ss:ebp+8]
	in		al,dx
	mov 	esp, ebp
	pop 	ebp
	ret

outportb:
	push 	ebp
	mov		ebp, esp
	mov 	dx, [ss:ebp+8]
	mov 	al, [ss:ebp+12]
	out 	dx, al
	mov 	esp, ebp
	pop 	ebp
	ret
