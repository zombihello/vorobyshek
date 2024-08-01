#ifndef CONSOLE_H
#define CONSOLE_H

#include "drivers/vga.h"

// Initialize console
void 	console_init();

// Clear console
void 	console_clear();

// Set current console color
void 	console_set_color( vgaColor_t textColor, vgaColor_t backgroundColor );

// Put char at same position and with same color
void 	console_put_char_at( char c, vgaColor_t textColor, vgaColor_t backgroundColor, size_t x, size_t y );

// Put char at current position and with current color
void 	console_put_char( char c );

#endif // !CONSOLE_H
