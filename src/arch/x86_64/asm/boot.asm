section .bss
	align 16
stack_bottom:
	resb 64
stack_top:

section .text
bits 32
global start
extern kernel_main

start:
	mov esp, stack_top							; Prepare the stack
	call checkMultiboot							; Check that we were loaded as multiboot
	mov dword [0xB8000], 0x2F4B2F4F	; Declare that we are 'OK'


	call kernel_main								; Call the higher C++ kernel
	hlt															; Stop the CPU.

errorOccurred:										; Declares that an error has occurred. Provide error code in al register
	mov dword[0xB8000], 0x4F524F45	; Colour 0x4F is used for all letters
	mov dword[0xB8004], 0x4F3A4F52	; Result is 'ERR: n' where n is the value of al
	mov dword[0xB8008], 0x4F204F20	;
	mov byte [0xB800A], al					;
	hlt															; Halt the CPU

checkMultiboot:										; Ensures a multiboot bootloader loaded the kernel
	cmp eax, 0x36D76289							; Compare eax and the magic number
	jne .noMultibootError						; Jump to the no multiboot error marker if they were not equal
	ret															;

.noMultibootError:								; The system was found not to be using multiboot. This is not allowed!
	mov al, "0"											; Give an error of code "0"
	jmp errorOccurred								; Throw the error
