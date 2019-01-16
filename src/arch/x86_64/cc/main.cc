#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "log.h"
#include "multiboot.h"
#include "memory.h"
#include "CLIGUI/terminal.h"
#include "util.h"
#include "interrupts.h"
#include "input/mouse.h"
#include "CLIGUI/textgui.h"

const char *stringg = "Hello, World! Num is: ";

class TestClass {
	char c;
	int i;
	void f () {}
};

void testMemory () {
	logLn ("Commencing memory test.");
	logAllocationTable();
	logInt (sizeof(struct heap_block));
	TestClass *t1 = new TestClass ();
	TestClass *t2 = new TestClass ();
	logHex((long long) t2);
	delete t1;
	logAllocationTable();
	TestClass *t3 = new TestClass ();
	logAllocationTable();
	delete t2;
	delete t3;
	logAllocationTable();
	logLn ("Done.");
}

void testTerminal () {
	Terminal *t = new Terminal();
	logLn ("Demoing terminal output...");
	for (int i = 0; i < 56; i++) {
		logInt (i);
		if (i % 2 == 0) {
			t->setActiveStyleFlag(0x2F);
		} else {
			t->setActiveStyleFlag(0x4F);
		}
		t->print(stringg);
		char str[4];
		intToString(str, i, 16);
		t->println(str);
		sleep (50);
	}
	logLn ("Done.");
}

extern "C" void kernel_main (int gdtPointer) {
	init_serial();
	logLn ("Welcome to the Neutron-OS kernel. This is a debug log.");

	log ("GDT Code Segment is declared as ");
	logInt (gdtPointer);

	log ("Preparing memory...             ");
	prepMemory();
	logLn ("Done.");

	logLn ("Initialising CLIGUI object... ");
	CLIGUI *cg = new CLIGUI;
	cg->setup();
	logLn ("Done.");

	logLn ("Transferring to console GUI control.");
	cg->main();

	// log ("Initialising IDT...             ");
	// initIDT(gdtPointer);
	// logLn ("Done.");

	// log ("Running interrupt test...       ");
	// asm (R"(
	// 	int $0x01
	// )");
	//logLn ("Success!");


	logLn ("Halting.");
	return;
}
