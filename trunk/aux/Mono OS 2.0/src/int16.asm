EXTERN KBBuffer
EXTERN KBBOffSet

SECTION .text

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
	push dword AH
	call int16
	pop AH
	iret
