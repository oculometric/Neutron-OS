#define PORT 0x3F8 /* COM1 */

uint8_t inb(uint16_t port);
void outb(unsigned short port, unsigned char val);

void init_serial();

int is_transmit_empty();

void write_serial(char a);

int serial_received();

char read_serial();

void logLn(char *s);
void log(char *s);
void logChar (char c);
void logInt (long int i);
void logHex (int i);
void logBool (bool b);

void fatal (char *msg);
void warn (char *msg);
