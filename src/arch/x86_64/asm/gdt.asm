section .early
bits 32

%define FLAG_CODE   0xa ; Read/Execute
%define FLAG_DATA   0x2 ; Read/Write

%define FLAG_USER   (1 << 4)
%define FLAG_SYSTEM (0 << 4)

%define FLAG_R0     (0 << 5)    ; Rings 0 - 3
%define FLAG_R1     (1 << 5)
%define FLAG_R2     (2 << 5)
%define FLAG_R3     (3 << 5)

%define FLAG_P      (1 << 7)    ; Present

%define FLAG_32     (1 << 14)   ; 1 for 32-bit compat
%define FLAG_4k     (1 << 15)   ; 4k page granularity
%define FLAGS_COMMON_32 (FLAG_USER | FLAG_R0 | FLAG_P | FLAG_32 | FLAG_4k)
%define FLAGS_CODE_32 (FLAG_CODE | FLAGS_COMMON_32)
%define FLAGS_DATA_32 (FLAG_DATA | FLAGS_COMMON_32)
%define FLAG_L          (1 << 13) ; 1 for 64-bit
%define FLAGS_CODE_64 (FLAG_USER | FLAG_R0 | FLAG_P | FLAG_L | FLAG_4k | FLAG_CODE)
%define FLAG_INTERRUPT  0xe

; 1 = FLAGS, 2 = BASE, 3 = LIMIT
%macro GDTENTRY 3
    dw  ((%3) & 0xFFFF)
    dw  ((%2) & 0xFFFF)
    db  (((%2) & 0xFF0000) >> 16)
    dw  ((%1) | (((%3) & 0xF0000) >> 8))
    db  (((%2) & 0xFF000000) >> 24)
%endmacro

global populateGDT
populateGDT:
    lgdt[GDTR]
    ret

global GDT
global GDTcode64
global GDTdata
global GDTR

align 8
section .rodata
GDT:																				; 64-bit GDT for long mode
	dq 0																			; Zero entry
GDTcode64: equ $ - GDT
	dq (1<<43) | (1<<44) | (1<<47) | (1<<53)	; Defines a number with bits 43, 44, 47, 53 set to 1
GDTdata: equ $ - GDT
	dq (1<<44) | (1<<47) | (1<<41)						; Defines a number with bits 44, 47, 41 set to 1
GDTR:
	dw $ - GDT - 1
	dq GDT
