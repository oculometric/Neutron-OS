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

// Generate a VGA entry from a colour and a character
uint16_t Terminal::make_vgaentry(char c, uint8_t color) {
  uint16_t c16 = c;
  uint16_t color16 = color;
  return c16 | color16 << 8;
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
  if (terminal_row == VGA_HEIGHT) {
    terminal_row = 0;
    resetTerminal();
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
    unsigned short charLoc = ((terminal_row * VGA_WIDTH) + terminal_column);
    videoMemStart[charLoc] = make_vgaentry (c, activeStyleFlag);
    terminal_column++;
    if (terminal_column > VGA_WIDTH) {
      moveToNextLine();
    }
  }
  updateCursorLocation();
}

void Terminal::deleteChars (int num) {
  while (num-- && terminal_column) {
    terminal_column--;
    unsigned short charLoc = ((terminal_row * VGA_WIDTH) + terminal_column);
    videoMemStart[charLoc] = make_vgaentry (' ', activeStyleFlag);
  }
  updateCursorLocation();
}

void Terminal::print (const char *s) {
  do {
    appendChar (*s);
  } while (*(s++) != '\0');
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
  num++;
  while (num-- && terminal_row) {
    unsigned short st = ((terminal_row * VGA_WIDTH));
    unsigned short charLoc;
    for (terminal_column = 0; terminal_column < VGA_WIDTH; terminal_column++) {
      charLoc = (st + terminal_column);
      videoMemStart[charLoc] = make_vgaentry (' ', activeStyleFlag);
    }
    terminal_row--;
  }
  terminal_column = 0;
  terminal_row++;
  updateCursorLocation();
}

void Terminal::resetTerminal () {
  for (int o = 0; o < (VGA_WIDTH * VGA_HEIGHT); o++) {
    videoMemStart[o] = make_vgaentry (' ', activeStyleFlag);
  }
}
