#include "interrupts.h"
#include "log.h"
#include "memory.h"
//
// idt_entry_t idtEntries[256];
// idt_ptr_t   *idtPtr;
//
// void initIDT(uint16_t gdt64Code) {
//    idtPtr->limit = sizeof(idt_entry_t) * 256 - 1;
//    idtPtr->base  = (uint64_t)idtEntries;
// 	 logHex ((uint64_t) idtPtr);
//
//    memset((char *)idtEntries, 0, sizeof(idt_entry_t)*256);
//    idt_set_gate(0, (uint64_t)isr0 , 0xf000, 0x8E);
//    idt_set_gate(1, (uint64_t)isr1 , 0xf000, 0x8E);
// 	 for (int i = 2; i < 256; i++) {
// 		 if (i != 31) {
// 			 idt_set_gate(i, (uint64_t)isr8 , 0xf000, 0x8E);
// 		 }
// 	 }
//
//    idt_set_gate(31, (uint64_t)isr31, 0xf000, 0x8E);
//
//    idtFlush((uint64_t)idtPtr);
// }
//
// void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
// 	uint32_t lower = base & 0xFFFFFFFF;
//    idtEntries[num].base_lo = lower & 0xFFFF;
//    idtEntries[num].base_hi = (lower >> 16) & 0xFFFF;
// 	 idtEntries[num].base_hier = (base >> 32) & 0xFFFFFFFF;
//
//    idtEntries[num].sel     = sel;
//    idtEntries[num].always0 = 0;
// 	 idtEntries[num].zero = 0;
//    // We must uncomment the OR below when we get to using user-mode.
//    // It sets the interrupt gate's privilege level to 3.
//    idtEntries[num].flags   = flags /* | 0x60 */;
// }
//
//
// void isrHandler (char intNum, char errCode) {
// 	logLn ("We made it!");
// 	logInt (intNum);
// }
//
void picAcknowledge (unsigned int interrupt) {
	if (interrupt < 0x20 || interrupt > 0x2F) return;
	if (interrupt < 0x28) outb(PIC1, 0x20); // ACK with PIC 1
	else outb(PIC2, 0x20); // ACK with PIC 2
}

unsigned char readScanCode () {
	return inb(KBD_DATA);
}

void setupPICs () {
	log ("Setting up PIC for keyboard...");
	// ICW1: Initialisation
	outb (PIC1, 0x11);
	outb (PIC2, 0x11);

	// ICW2: Remap IDT
	outb (PIC1 + 1, 0x20);
	outb (PIC2 + 1, 0x28);

	// ICW3: Setup cascading
	outb (PIC1 + 1, 0x04);
	outb (PIC2 + 1, 0x02);

	// ICW4: Environment information
	outb (PIC1 + 1, 0x01);
	outb (PIC2 + 1, 0x01);

	// Mask interrupts
	outb (PIC1 + 1, 0xFF);
	outb (PIC2 + 1, 0xFF);
	logLn ("Done.");
}

void setTypmaticSpeed (uint8_t value) {
	outb (KBD_COMMAND, 0xF3); // Tell the keyboard to set the typematic speed
	while (inb (KBD_COMMAND) != ACK); // Wait for ACK
	outb (KBD_COMMAND, value); // Set the speed to the data provided
}

void setScanCodeSet (int i) {
	string *s = new string (i, 10);

	log ("Setting scan code set to "); log (s);
	if (i < 1 || i > 3) return;
	outb (KBD_COMMAND, 0xF0); // Tell the keyboard to set the scancode set
	while (inb (KBD_COMMAND) != ACK) logHex (inb(KBD_COMMAND)); // Wait for ACK
	outb(KBD_COMMAND, i);
	delete s;
	logLn ("Done.");
}

void keyboardInterruptHandle () {
	// TODO: Handle keyboard input here
	logLn ("Here we are");
}
