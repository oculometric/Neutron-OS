section .early
bits 32

global start
global errorOccurred

extern longModeStart

extern populateGDT
extern populateIDT

extern GDTR
extern GDTdata
extern GDTcode64
extern GDT

start:
	cli
	mov esp, stack_top							; Prepare the stack
	mov edi, GDTcode64										; Move multiboot information pointer into ebx
	push eax
	push ebx
	call checkMultiboot							; Check that we were loaded as multiboot
	call checkCpuid									; Make sure the CPU supports CPUID
	call checkLongMode							; Check that long more is available
	call preparePageTables					; Set up paging
	call enablePaging

	;lgdt [gdt64.pointer]						; Load the 64-bit long mode GDT
	;call EnableA20
	call populateGDT
	mov eax, GDTdata
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax

	mov dword [0xB8000], 0x2F4B2F4F	; Declare that we are 'OK'
	jmp GDTcode64:longModeStart		; Perform the long-awaited far jump to the start of long mode code

hostGDT:
	call populateIDT
	jmp $

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

checkCpuid:							; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
												; in the FLAGS register. If we can flip it, CPUID is available.
	pushfd								; Copy FLAGS in to eax via stack
	pop eax
	mov ecx, eax					; Copy to ECX as well for comparing later on
	xor eax, 1 << 21			; Flip the ID bit
	push eax							; Copy EAX to FLAGS via the stack
	popfd

	pushfd								; Copy FLAGS back to eax (with the flipped bit if CPUID is supported)
	pop eax

	push ecx							; Restore FLAGS from the old version stored in ecx (i.e. flipping the ID bit back if it was ever flipped).
	popfd

	cmp eax, ecx					; Compare eax and ecx. If they are equal then that means the bit wasn't flipped, and CPUID isn't supported.
	je .noCpuid
	ret
.noCpuid:								; The system was found not to be capable of CPUID.
	mov al, "1"						; Give an error code of "1"
	jmp errorOccurred

checkLongMode:				; test if extended processor info in available
	mov eax, 0x80000000		; implicit argument for cpuid
	cpuid									; get highest supported argument
	cmp eax, 0x80000001		; it needs to be at least 0x80000001
	jb .noLongMode				; if it's less, the CPU is too old for long mode

	; use extended info to test if long mode is available
	mov eax, 0x80000001		; argument for extended processor info
	cpuid									; returns various feature bits in ecx and edx
	test edx, 1 << 29			; test if the LM-bit is set in the D-register
	jz .noLongMode				; If it's not set, there is no long mode
	ret
.noLongMode:						; Long mode is not available
	mov al, "2"						; Give an error code of "2"
	jmp errorOccurred

preparePageTables:
	mov eax, p3_table			; map first P4 entry to P3 table
	or eax, 0b11					; present + writable
	mov [p4_table], eax

	mov eax, p2_table			; map first P3 entry to P2 table
	or eax, 0b11					; present + writable
	mov [p3_table], eax

	mov ecx, 0						; Counter
.mapP2Table:						; Map ecx-th P2 entry to a huge page that starts at 2MiB + ecx
	mov eax, 0x200000			;2MiB
	mul ecx								; start address of ecx-th page
	or eax, 0b10000011		; present + writable + huge
	mov [p2_table + ecx * 8], eax ; map ecx-th entry
	inc ecx								; Increment counter
	cmp ecx, 512					; If counter is 512, the table has been fully mapped
	jne .mapP2Table				; Otherwise map the next entry

	ret

enablePaging: 					; Enable memory paging
	mov eax, p4_table			; Load P4's address into the cr3 register
	mov cr3, eax

	mov eax, cr4
	or eax, 1 << 5				; Set the fifth bit (PAE-flag) in cr4
	mov cr4, eax

	mov ecx, 0xC0000080
	rdmsr									; Read the EFER from MSR (model specific register)
	or eax, 1 << 8				; Set the long mode bit (bit 8)
	wrmsr									; Write back to the MSR

	mov eax, cr0
	or eax, 1 << 31				; Enable paging
	mov cr0, eax

	ret

EnableA20:
	in al, 0x92         ; A20, using fast A20 gate
	mov cl, al
	and cl, 2
	jnz .skip            ; if a20 bit seems set, don't touch it
	or al, 2
	out 0x92, al
.skip:
	ret

section .bss
align 4096
p4_table:
  resb 4096
p3_table:
  resb 4096
p2_table:
  resb 4096
stack_bottom:
	resb 4096 * 4
stack_top:
