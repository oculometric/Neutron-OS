#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "log.h"
#include "memory.h"
#include "string.h"
#include "terminal.h"
#include "util.h"

const char *string = "Hello, World! Num is: ";

extern "C" void kernel_main (void) {
	init_serial();
	logLn ("Welcome to the Neutron-OS kernel. This is a debug log.");

	log ("Initialising terminal object... ");
	Terminal t = Terminal();
	logLn ("Done.");

	log ("Clearing terminal...            ");
	t.resetTerminal();
	logLn ("Done.");

	logLn ("Demoing terminal output...");
	for (int i = 0; i < 56; i++) {
		logInt (i);
		if (i % 2 == 0) {
			t.setActiveStyleFlag(0x2F);
		} else {
			t.setActiveStyleFlag(0x4F);
		}
		t.print(string);
		char str[4];
		intToString(str, i, 16);
		t.println(str);
		sleep (50);
	}
	logLn ("Done.");

	logLn ("Halting.");
	return;
}
