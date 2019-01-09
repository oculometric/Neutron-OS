#include <stdint.h>
#include <stddef.h>

// A struct describing an interrupt gate.
struct idt_entry_struct
{
   uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
   uint16_t sel;                 // Kernel segment selector.
   uint8_t  always0;             // This must always be zero.
   uint8_t  flags;               // More flags. See documentation.
   uint16_t base_hi;             // The upper 16 bits of the address to jump to.
	 uint32_t base_hier;					 // The top 32 bits of the address to jump to
	 uint32_t zero;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
   uint16_t limit;
   uint64_t base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

extern "C" void isr0 ();
extern "C" void isr1 ();
extern "C" void isr8 ();
extern "C" void isr31 ();
extern "C" void idtFlush (uint64_t);

void initIDT(uint16_t);
void idt_set_gate(uint8_t,uint64_t,uint16_t,uint8_t);
extern "C" void isrHandler (char intNum, char errCode);

#ifndef KBD_DATA
#define KBD_DATA 0x60
#define KBD_COMMAND 0x64
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
void setTypmaticSpeed (uint8_t value);
void setupPICs ();
void setScanCodeSet (int i);
KeypressPacket getCurrentScancode ();
