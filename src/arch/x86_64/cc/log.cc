#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "log.h"
#include "string.h"

// Get a byte of input
uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

// Set a byte of output
void outb(unsigned short port, unsigned char val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void init_serial() {
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit // FIXME: this line causes a jump to 0x0000000019c80010 outside ROM
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void write_serial(char a) {
  while (is_transmit_empty() == 0);
  outb(PORT, a);
}

int serial_received() { return inb(PORT + 5) & 1; }

char read_serial() {
   while (serial_received() == 0);
   return inb(PORT);
}

void logLn(char *s) {
  for (int i = 0; s[i] != NULL; i++) {
    write_serial(s[i]);
  }
  write_serial(0x0A);
}

void log(char *s) {
  for (int i = 0; s[i] != NULL; i++) {
    write_serial(s[i]);
  }
}

void logChar (char c) {
	write_serial(c);
	write_serial(0x0A);
}

void logInt (long int i) {
  long int c = i;
  int r = 0;
  do {
    r++;
  } while (c /= 10);
	char s[r] = {0x0};
	intToString(s, i, 10);
  logLn (s);
}

void logHex (int i) {
  int c = i;
  int r = 0;
  do {
    r++;
  } while (c /= 16);
	char s[r] = {0x0};
	intToString(s, i, 16);
	log ("0x");
  logLn (s);
}

void logBool (bool b) {
  char *s = b ? (char *)"true" : (char *)"false";
  logLn (s);
}

//Log that a fatal error occurred
void fatal (char *msg) {
  logLn ("==========FATAL==========");
  logLn (msg);
  logLn ("=========================");
	//shouldContinue = false;
}

// Log a warning
void warn (char *msg) {
  logLn ("==========WARNING==========");
  logLn (msg);
  logLn ("===========================");
}
