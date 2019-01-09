;global load_idt

;idt:
;	dw 2048
;	dd 0x0

;load_idt:
;	lidt [idt]
;	ret
bits 64
extern isrHandler
global isr1

global idtFlush
idtFlush:
	cli
	mov eax, [esp+4]
	lidt [eax]
	ret

%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 8
ISR_NOERRCODE 31

isr_common_stub:
	call isrHandler
	sti
	iret
