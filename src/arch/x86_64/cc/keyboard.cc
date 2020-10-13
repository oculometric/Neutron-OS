#include "keyboard.h"
#include "log.h"
#include "memory.h"

void picAcknowledge (unsigned int interrupt) {
	if (interrupt < 0x20 || interrupt > 0x2F) return;
	if (interrupt < 0x28) outb(PIC1, 0x20); // ACK with PIC 1
	else outb(PIC2, 0x20); // ACK with PIC 2
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

void outBufferWait () {
	while (inb(KBD_COMMAND) & 1);
}

void ackWait () {
	while (inb(KBD_DATA) != ACK);
}

bool getStatus () {
	return inb (KBD_COMMAND) & (1 << 2);
}

// FIXME: Seemingly not showing up in emulator
void setLEDState (int n, bool on) {
	outBufferWait();
	outb (KBD_DATA, 0xED);
	ackWait();
	outBufferWait();
	outb (KBD_DATA, on << n);
	ackWait();
}

void setScanCodeSet (int i) {
	log ("Setting scan code set to "); logInt (i); logLn();
	if (i < 1 || i > 3) return;
	outBufferWait();
	outb (KBD_DATA, 0xF0); // Tell the keyboard to set the scancode set
	ackWait(); // Wait for ACK
	outBufferWait();
	outb(KBD_DATA, i);
	ackWait();
	logLn ("Done.");
}

int getScanCodeSet () {
	outBufferWait();
	outb (KBD_DATA, 0xF0); // Tell the keyboard to set the scancode set
	ackWait(); // Wait for ACK
	outBufferWait();
	outb(KBD_DATA, 0);
	ackWait();
	while (!inb(KBD_COMMAND) & (1 << 1));
	char res = inb (KBD_DATA);
	if (res == 0x43) return 1;
	if (res == 0x41) return 2;
	if (res == 0x3f) return 3;
	return 0;
}

void keyboardInterruptHandle () {
	// TODO: Handle keyboard input here
	logLn ("Here we are");
}
