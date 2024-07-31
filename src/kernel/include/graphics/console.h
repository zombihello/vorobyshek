#ifndef CONSOLE_H
#define CONSOLE_H

#include "graphics/vga.h"

// Initialize console
void 	ConsoleInit();

// Clear console
void 	ConsoleClear();

// Set current console color
void 	ConsoleSetColor( vgaColor_t textColor, vgaColor_t backgroundColor );

// Put char at same position and with same color
void 	ConsolePutCharAt( char c, vgaColor_t textColor, vgaColor_t backgroundColor, size_t x, size_t y );

// Put char at current position and with current color
void 	ConsolePutChar( char c );

// Write string at current position
void 	ConsoleWrite( const char* pStr );

#endif // !CONSOLE_H
