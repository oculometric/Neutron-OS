global longModeStart
extern kernel_main

section .text
bits 64
longModeStart:
	mov rax, 0x2F592F412F4B2F4F			; A quadword meaning 'OKAY'
	mov qword [0xB8000], rax
	call kernel_main								; Call the higher C++ kernel
	mov word [0xB8000], 0x2F44
	mov word [0xB8002], 0x2F4F
	mov word [0xB8004], 0x2F4E
	mov word [0xB8006], 0x2F45

	hlt															; Stop the CPU.
