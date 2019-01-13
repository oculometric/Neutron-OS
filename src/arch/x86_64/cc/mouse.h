#include <stdint.h>

#ifndef MOUSEPACKET
#define MOUSEPACKET
typedef struct MousePacket {
	int xMovement;
	bool xIsNegative;
	int yMovement;
	bool yIsNegative;
	bool leftButton;
	bool rightButton;
	bool middleButton;
	int scroll;
};
#endif

void initMouse ();

void setEnableStreaming (bool b);

void setSampleRate (int i);
void setResolution (int i);
void setScaling (bool nonlinear);

uint32_t getStatus ();

bool isPacketAvailable ();
void poll (MousePacket *mp);

#ifndef MOUSE_DATA
#define MOUSE_DATA 0x60
#define MOUSE_COMMAND 0x64
#define ACK 0xFA
#endif
