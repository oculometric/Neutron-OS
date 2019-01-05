#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "log.h"
#include "multiboot.h"
#include "memory.h"
#include "string.h"
#include "terminal.h"
#include "util.h"

const char *string = "Hello, World! Num is: ";

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

extern "C" void kernel_main (multiboot_info* mbd, unsigned int magic) {
	init_serial();
	logLn ("Welcome to the Neutron-OS kernel. This is a debug log.");

	log ("Preparing memory...             ");
	prepMemory();
	logLn ("Done.");

	log ("Initialising terminal object... ");
	Terminal *t = new Terminal();
	logLn ("Done.");

	log ("Clearing terminal...            ");
	t->resetTerminal();
	logLn ("Done.");

	logLn ("Demoing terminal output...");
	for (int i = 0; i < 56; i++) {
		logInt (i);
		if (i % 2 == 0) {
			t->setActiveStyleFlag(0x2F);
		} else {
			t->setActiveStyleFlag(0x4F);
		}
		t->print(string);
		char str[4];
		intToString(str, i, 16);
		t->println(str);
		sleep (50);
	}
	logLn ("Done.");

	logAllocationTable();
	delete t;
	logAllocationTable();

	logLn ("Halting.");
	return;
}
