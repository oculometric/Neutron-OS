#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "log.h"
#include "multiboot.h"
#include "memory.h"
#include "terminal.h"
#include "util.h"

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

	string *s1 = new string ("Hello, World!");
	string *s2 = new string (" and everyone.");
	string *s3 = s1->appending(s2);
	string *s4 = new string ("Hi,");
	string *s5 = new string (" everyone.");
	string *s6 = new string ();
	s6->copyFrom("Hello, World");
	s6->append(s5);
	s4->append(" no one.");

	logAllocationTable();

	logLn (s1);
	logLn (s2);
	logLn (s3);
	logLn (s4);
	logLn (s5);
	logLn (s6);

	t->println(s3);
	t->println(s4);

	delete t;
	delete s1;
	delete s2;
	delete s3;
	delete s4;
	delete s5;
	delete s6;
	updateAllocationTable();
	logAllocationTable();

	logLn ("Halting.");
	return;
}
