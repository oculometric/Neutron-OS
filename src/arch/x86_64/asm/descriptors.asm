#define FLAG_CODE   0xa //Read/Execute
#define FLAG_DATA   0x2 //Read/Write
 
#define FLAG_USER   (1 << 4)
#define FLAG_SYSTEM (0 << 4)
 
#define FLAG_R0     (0 << 5)    // Rings 0 - 3
#define FLAG_R1     (1 << 5)
#define FLAG_R2     (2 << 5)
#define FLAG_R3     (3 << 5)
 
#define FLAG_P      (1 << 7)    // Present
 
#define FLAG_32     (1 << 14)   // 1 for 32-bit compat
#define FLAG_4k     (1 << 15)   // 4k page granularity
#define FLAGS_COMMON_32 (FLAG_USER | FLAG_R0 | FLAG_P | FLAG_32 | FLAG_4k)
#define FLAGS_CODE_32 (FLAG_CODE | FLAGS_COMMON_32)
#define FLAGS_DATA_32 (FLAG_DATA | FLAGS_COMMON_32)
#define FLAG_L          (1 << 13) // 1 for 64-bit
#define FLAGS_CODE_64 (FLAG_USER | FLAG_R0 | FLAG_P | FLAG_L | FLAG_4k | FLAG_CODE)
#define FLAG_INTERRUPT  0xe
#define FLAG_R0     (0 << 5)    // Rings 0 - 3
#define FLAG_P      (1 << 7)
 
#define IDT_ENTRIES     256

/* 1 = FLAGS, 2 = BASE, 3 = LIMIT */
section .text

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

section .rodata

align 8
GDT:
    /* NULL descriptor */
    GDTENTRY    0, 0x0, 0x0
    /* Code descriptor (Compat) */
CODE_SEL_32 EQU $-GDT
    GDTENTRY    FLAGS_CODE_32, 0x0, 0xFFFFF
    /* Data descriptor (Compat) */
DATA_SEL EQU $-GDT
    GDTENTRY    FLAGS_DATA_32, 0x0, 0xFFFFF
    /* Code descriptor (Long) */
CODE_SEL_64 EQU $-GDT
    GDTENTRY    FLAGS_CODE_64, 0x0, 0xFFFFF
GDTEND:

align 8
GDTR:
dw (GDTEND - GDT - 1)
dq GDT