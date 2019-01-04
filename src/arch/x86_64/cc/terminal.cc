#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "terminal.h"
#include "log.h"

// ALL OF THESE FUNCTIONS REQUIRE THE SYSTEM TO BE IN VGA GRAPHICS MODE 0x3H

// Generate foreground/background pair
uint8_t Terminal::make_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

unsigned short Terminal::makeVGA (unsigned short col, char c) {
	return (col << 8) | c;
}

// Move the cursor to the end of the text
void Terminal::updateCursorLocation() {
  unsigned short cursorLoc = ((terminal_row * VGA_WIDTH) + terminal_column);
  // cursor LOW port to vga INDEX register
  outb(0x3D4, 0x0F);
  outb(0x3D5, (unsigned char)(cursorLoc));
  // cursor HIGH port to vga INDEX register
  outb(0x3D4, 0x0E);
  outb(0x3D5, (unsigned char)((cursorLoc >> 8)));
}

void Terminal::moveToNextLine () {
  terminal_row++;
  terminal_column = 0;
  if (terminal_row > VGA_HEIGHT) {
    terminal_row = VGA_HEIGHT;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
			videoMemStart[i] = videoMemStart[i+VGA_WIDTH];
		}
  }
  updateCursorLocation();
}

void Terminal::appendChar (char c) {
  if (c == '\n') {
    moveToNextLine ();
  } else if (c == '\t') {

  } else if (c == '\b') {
    deleteChars (1);
  } else {
		int place = (VGA_WIDTH * terminal_row) + terminal_column;
    videoMemStart[place] = makeVGA(activeStyleFlag, c);
    terminal_column++;
    if (terminal_column > VGA_WIDTH) {
      moveToNextLine();
    }
  }
  updateCursorLocation();
}

void Terminal::deleteChars (int num) {
	unsigned short c = makeVGA(0x00, ' ');
	int startPoint = (VGA_WIDTH * terminal_row) + terminal_column;
  while (num-- && terminal_column--) {
    videoMemStart[startPoint] = c;
		startPoint--;
  }
  updateCursorLocation();
}

void Terminal::print (const char *s) {
  do {
    appendChar (*s);
  } while (*(++s) != '\0');
}

void Terminal::println (const char *s) {
  print (s);
  moveToNextLine();
  updateCursorLocation();
}

void Terminal::setActiveStyleFlag (char flag) {
  activeStyleFlag = flag;
}

void Terminal::deleteLines (int num) {
	unsigned short c = makeVGA(0x00, ' ');
  while (num-- && terminal_row--) {
		int rowStart = (VGA_WIDTH * (terminal_row));
    int rowEnd = (VGA_WIDTH * (terminal_row+1));
		for (int i = rowStart; i < rowEnd; i++) {
			videoMemStart[i] = c;
		}
  }
  terminal_column = 0;
  terminal_row++;
  updateCursorLocation();
}

void Terminal::resetTerminal () {
	unsigned short c = makeVGA(0x00, ' ');
	int numPlaces = VGA_WIDTH * VGA_HEIGHT;
	for (int i = 0; i < numPlaces; i++) {
		videoMemStart[i] = c;
	}
	terminal_column = 0;
	terminal_row = 0;
	updateCursorLocation();
}
