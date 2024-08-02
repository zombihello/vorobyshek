#ifndef CONSOLE_H
#define CONSOLE_H

#include "drivers/vga.h"

// Cursor position in the console
typedef struct
{
	size_t 		x;
	size_t 		y;
} consoleCursorPos_t;

// Initialize console
void 					console_init();

// Clear console
void 					console_clear();

// Clear specific line in console
void 					console_clear_line( size_t line );

// Set current console color
void 					console_set_color( vgaColor_t textColor, vgaColor_t backgroundColor );

// Set cursor position
void 					console_set_cursor_pos( consoleCursorPos_t cursorPosition );

// Get cursor position
consoleCursorPos_t  	console_get_cursor_pos();

// Put char at same position and with same color
void 					console_put_char_at( char c, vgaColor_t textColor, vgaColor_t backgroundColor, size_t x, size_t y );

// Put char at current position and with current color
void 					console_put_char( char c );

#endif // !CONSOLE_H
