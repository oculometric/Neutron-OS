#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "log.h"
#include "memory.h"
#include "string.h"
#include "terminal.h"

extern "C" void kernel_main (void) {
	init_serial();

	Terminal t = Terminal();

	t.println("At least the terminal is working!");

	logLn("We're up and running!");

	while (true);
}
