// #include "interrupts.h"
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