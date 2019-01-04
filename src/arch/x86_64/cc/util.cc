#include "util.h"

void sleep (long reps) {
	long del = reps * 1000000;
	volatile int sink;
	do {
		sink = 0;
	} while (--del > 0);
	(void)sink;
}
