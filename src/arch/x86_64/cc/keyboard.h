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
void keyboardInterruptHandle ();
typedef struct KeypressPacket {
	bool down;
	char character;
};
KeypressPacket getCurrentScancode ();

unsigned char readScanCode ();
void setupPICs ();

void setLEDState (int n, bool on);
void setScanCodeSet (int i);
int getScanCodeSet ();
void identifyKeyboard ();
void setTypematicSpeed (char repeat, char delay);
void setSendScanCodes (bool on);
void resetParameters ();
void setKeyMode (int mode);
void setKeyMode (int mode, char key);
char restart ();