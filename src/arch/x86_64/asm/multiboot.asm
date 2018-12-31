section .multiboot_header
headerStart:
    dd 0xe85250d6                ; magic number
    dd 0                         ; start in 32-bit protected, we can then jump up into long mode
    dd headerEnd - headerStart ; header length
    ; checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (headerEnd - headerStart))

    ; insert optional multiboot tags here

    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
headerEnd:
