#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "log.h"


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
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
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

void logLn() {
  write_serial(0x0A);
}

void log(char *s) {
  for (int i = 0; s[i] != NULL; i++) {
    write_serial(s[i]);
  }
}

void logChar (char c) {
	write_serial(c);
}

void logInt (long int i) {
  long int c = i;
  int r = 0;
  do {
    r++;
  } while (c /= 10);
	char s[r] = {0x0};
	intToString(s, i, 10);
  log (s);
}

void logHex (long long int i) {
  int c = i;
  int r = 0;
  do {
    r++;
  } while (c /= 16);
	char s[r] = {0x0};
	intToString(s, i, 16);
	log ("0x");
  log (s);
}

void logBool (bool b) {
  char *s = b ? (char *)"true" : (char *)"false";
  log (s);
}

void logLn (string *s) {
	logLn (s->getData());
}
void log (string *s) {
	log (s->getData());
}

//Log that a fatal error occurred
void fatal (char *msg) {
  logLn ("==========FATAL==========");
  logLn (msg);
  logLn ("=========================");
	asm (R"(
		movq $0x4F524F45, 0xB8000 # ER
		movq $0x4F3A4F52, 0xB8004 # R:
		movq $0x4F204F20, 0xB8008	#
		movq $0x4F454F53, 0xB800C # SE
		movq $0x4F204F45, 0xB8010 # E
		movq $0x4F4F4F4C, 0xB8014 # LO
		movq $0x4F204F47, 0xB8018 # G
		movq $0x4F494F46, 0xB801C # FI
		movq $0x4F454F4C, 0xB8020 # LE

		hlt	)");
}

// Log a warning
void warn (char *msg) {
  logLn ("==========WARNING==========");
  logLn (msg);
  logLn ("===========================");
}
