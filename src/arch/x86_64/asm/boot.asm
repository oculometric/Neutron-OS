global start

section .text
bits 32
start:
	; Just display OK and stop
	mov dword [0xB8000], 0x2F4B2F4F
	hlt
