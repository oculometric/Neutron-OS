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
	int timer = 10000;
	while (inb(KBD_DATA) != ACK && timer > 0) timer--;
	if (timer == 0) logLn ("Keyboard failed to ACK in time.");
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
	outBufferWait(); // Wait for the output buffer to be clear
	outb (KBD_DATA, 0xF0); // Tell the keyboard to set the scancode set
	ackWait(); // Wait for ACK
	outBufferWait();
	outb(KBD_DATA, 0);
	ackWait();
	while (!(inb(KBD_COMMAND) & (1 << 0)));
	char res = inb (KBD_DATA);
	if (res == 0x43) return 1;
	if (res == 0x41) return 2;
	if (res == 0x3f) return 3;
	return 0;
}

void identifyKeyboard (char* p) {
	setScanningOn (false);
	outBufferWait();
	outb (KBD_DATA, 0xF2);
	ackWait();
	logLn (new string (inb (KBD_COMMAND), 2));
	int offset = 0;
	while (inb (KBD_COMMAND) & (1 << 0)) {
		logLn (new string (inb (KBD_COMMAND), 2));
		p[offset] = inb (KBD_DATA);
		offset++;
	}
	setScanningOn (true);
}

void setScanningOn (bool on) {
	log ("Setting scanning to "); logBool (on); log ("...    ");
	outBufferWait ();
	if (on) {
		outb (KBD_DATA, 0xF4);
	} else {
		outb (KBD_DATA, 0xF5);
	}
	ackWait();
	logLn ("Done.");
}

void setTypematicSpeed (char repeat, char delay) {
	outBufferWait();
	outb (KBD_DATA, 0xF3);
	ackWait();
	char outbyte = 0;
	if (repeat >= 30) outbyte |= 0;
	else if (repeat <= 2) outbyte |= 31;
	else outbyte |= (char)((30-repeat)/*/0.903225806451613*/); // TODO: Improve conversion
	outbyte |= (delay & 0b00000011) << 5;
	outBufferWait();
	outb (KBD_DATA, outbyte);
	ackWait();
}

void resetParameters () {
	outBufferWait();
	outb (KBD_DATA, 0xF6);
	ackWait();
}

/** Set the mode in which keys operate.
 * 0 - All keys typematic only
 * 1 - All keys make/release
 * 2 - All keys make only
 * 3 - All keys make/release/typematic
 **/
void setKeyMode (unsigned int mode) {
	if (getScanCodeSet() != 3) return;
	if (mode > 3) return;
	outBufferWait();
	if (mode == 0) {
		outb (KBD_DATA, 0xF7);
	} else if (mode == 1) {
		outb (KBD_DATA, 0xF8);
	} else if (mode == 2) {
		outb (KBD_DATA, 0xF9);
	} else if (mode == 3) {
		outb (KBD_DATA, 0xF7);
	}
	ackWait();
}