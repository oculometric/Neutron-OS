#include "../string.h"
#include <stddef.h>
#include <stdint.h>

#ifndef TERMINAL
#define TERMINAL
enum vga_color {
  COLOR_BLACK = 0,
  COLOR_BLUE = 1,
  COLOR_GREEN = 2,
  COLOR_CYAN = 3,
  COLOR_RED = 4,
  COLOR_MAGENTA = 5,
  COLOR_BROWN = 6,
  COLOR_LIGHT_GREY = 7,
  COLOR_DARK_GREY = 8,
  COLOR_LIGHT_BLUE = 9,
  COLOR_LIGHT_GREEN = 10,
  COLOR_LIGHT_CYAN = 11,
  COLOR_LIGHT_RED = 12,
  COLOR_LIGHT_MAGENTA = 13,
  COLOR_LIGHT_BROWN = 14,
  COLOR_WHITE = 15,
};

// terminaloutput.cc and terminalinput.cc
class Terminal {
public:
  void print (const char *s);
  void println (const char *s);
	void print (string *s);
  void println (string *s);
  void deleteChars (int num);
  void deleteLines (int num);
  void setActiveStyleFlag (char flag);
  uint8_t make_color(enum vga_color fg, enum vga_color bg);
  void resetTerminal ();
	unsigned short makeVGA (unsigned short col, char c);
	void setStyleFlag (int col, int row, char flag);
	char getActiveStyleFlag ();
	char getStyleFlag (int col, int row);

	void setChar (int col, int row, char c, char flag);
	void setChars (int colStart, int rowStart, char *c, char flag);

	static const size_t VGA_WIDTH = 80;
	static const size_t VGA_HEIGHT = 25;

	void enableTextCursor ();
	void disableTextCursor ();

private:
  void appendChar (char c);
  void moveToNextLine ();
	unsigned short *videoMemStart = (unsigned short *)0xB8000;
  char activeStyleFlag = 0x07;
  int terminal_row = 0;
	int terminal_column = 0;
	bool cursorEnabled = true;

  void updateCursorLocation ();
};
#endif
