section .early
bits 32

%define IDT_ENTRIES     256

%macro ISR 1
isr%1:
    jmp $
%endmacro

makeISRs:
	%assign i 0
	%rep IDT_ENTRIES
	ISR i
	%assign i (i+1)
	%endrep

%define ISR_SIZE (isr1 - isr0)

%macro IDTENTRY 0
    DD 0xabcdefab
    DD 0xabcdefab
    DD 0xabcdefab
    DD 0xabcdefab
%endmacro

global populateIDT
populateIDT:
    ; TODO:

global IDT
global IDTR

align 8
IDT:
	%assign i 0
	%rep IDT_ENTRIES
	IDTENTRY
	%assign i (i+1)
	%endrep
IDTEND:


align 8
IDTR:
	DW (IDTEND - IDT - 1)
	DQ IDT
