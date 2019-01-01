#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "log.h"
#include "memory.h"
#include "string.h"

extern "C" void kernel_main (void) {
	char *vidmem = (char *)0xB8000;
	vidmem[2*2] = '!';
	char *str = "Hello, World!\0";
	int len = strlen (str);
	init_serial();
	logInt (len);
	memcpy(vidmem+(2*3), str, len);

	logLn("We're up and running!");
}
