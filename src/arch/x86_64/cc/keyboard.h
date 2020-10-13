#include <stdint.h>
#include <stddef.h>

#ifndef KBD_DATA
#define KBD_DATA 0x60
#define KBD_COMMAND 0x64
#define RESEND 0xFE
#define ACK 0xFA
#define PIC1 0x20
#define PIC2 0xA0
#endif


// Keyboard stuff
void keyboardInterruptHandle (); // TODO: Unimplemented
typedef struct KeypressPacket {
	bool down;
	char character;
};
KeypressPacket pollScancode (); // TODO: Unimplemented

void setupPICs ();

void setLEDState (int n, bool on);
void setScanCodeSet (int i);
int getScanCodeSet ();
void identifyKeyboard (char* p);
void setTypematicSpeed (char repeat, char delay);
void setScanningOn (bool on);
void resetParameters ();
void setKeyMode (unsigned int mode); // TODO: Unimplemented
void setKeyMode (unsigned int mode, char key); // TODO: Unimplemented
char restart (); // TODO: Unimplemented