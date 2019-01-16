#include "mouse.h"
#include "../log.h"

void initMouse () {
	setEnableStreaming(true);
	setResolution(0x00); // 1 count per mm
	setScaling(false);   // Linear scaling
}

bool isPacketAvailable () {
	uint8_t i = inb(MOUSE_COMMAND);
	if (i == 0xAA) {
		initMouse();
		return false;
	}
	if (!(i & 1)) return false;
	if (i & (1 << 5)) return true;
	return false;
}

void setEnableStreaming (bool b) {
	log ("Enabling mouse packet streaming... ");
	while (inb (MOUSE_COMMAND) & 1);
	outb(MOUSE_COMMAND, 0xD4);
	while (inb (MOUSE_COMMAND) & 1);
	if (b) {
		outb(MOUSE_DATA, 0xF4);
	} else {
		outb(MOUSE_DATA, 0xF5);
	}
	while (inb (MOUSE_DATA) != ACK);
	logLn ("Done");
}

void setResolution (int i) {
	log ("Setting mouse motion resolution... ");
	while (inb (MOUSE_COMMAND) & 1);
	outb(MOUSE_COMMAND, 0xD4);
	while (inb (MOUSE_COMMAND) & 1);
	outb(MOUSE_DATA, 0xE8);
	while (inb (MOUSE_DATA) != ACK);
	outb(MOUSE_DATA, i);
	while (inb (MOUSE_DATA) != ACK);
	logLn ("Done");
}

unsigned char getByte () {
	while (!(inb (MOUSE_COMMAND) & 1));
	while (!(inb (MOUSE_COMMAND) & 1));
	return inb (MOUSE_DATA);
}

void poll (MousePacket *mp) {
	int b1 = getByte();
	int b2 = getByte();
	int b3 = getByte();
	//logLn (new string (b1, 2));
	//logLn (new string (b2, 2));
	//logLn (new string (b3, 2));
	mp->leftButton = b1 & 1;
	mp->rightButton = b1 & (1 << 1);
	mp->middleButton = b1 & (1 << 2);

	bool xIsNegative = (b1 & (1 << 4));
	bool yIsNegative = (b1 & (1 << 5));
	mp->xMovement = b2;
	mp->xIsNegative = false;
	mp->yMovement = b3;
	mp->yIsNegative = false;
	if (xIsNegative) {
		mp->xMovement = (255 - b2);
		mp->xIsNegative = true;
	}
	if (yIsNegative) {
		mp->yMovement = (256 - b3);
		mp->yIsNegative = true;
	}
}

void setScaling (bool nonlinear) {
	log ("Setting mouse motion scaling... ");
	while (inb (MOUSE_COMMAND) & 1);
	outb(MOUSE_COMMAND, 0xD4);
	while (inb (MOUSE_COMMAND) & 1);
	if (nonlinear) {
		outb(MOUSE_DATA, 0xE7);
	} else {
		outb(MOUSE_DATA, 0xE6);
	}

	while (inb (MOUSE_DATA) != ACK);
	logLn ("Done");
}
