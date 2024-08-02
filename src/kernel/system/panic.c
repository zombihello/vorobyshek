#include <stdarg.h>

#include "drivers/serial_port.h"
#include "system/hal.h"
#include "graphics/console.h"
#include "memory/memory.h"
#include "utils/printf.h"
#include "utils/string.h"

// Max panic message size
#define MAX_PANIC_MESSAGE_SIZE 			4096 		// 4KB

void panic( const char* pFormat, ... )
{
	// Initialize console to print panic message	
	const size_t 			centerWidth 		= g_VGAFrameBuffer.width / 2;
	consoleCursorPos_t 		cursorPosition 		= 
	{ 
		.x = centerWidth - 4,
		.y = 0 
	};

	// Clear console
	console_set_color( VGA_COLOR_WHITE, VGA_COLOR_BLACK );
	console_clear();
	
	// Print panic header
	console_set_color( VGA_COLOR_RED, VGA_COLOR_WHITE );
	console_clear_line( 0  );
	console_set_cursor_pos( cursorPosition );
	printf( "PANIC!" );
	console_set_color( VGA_COLOR_WHITE, VGA_COLOR_BLACK );

	// Set cursor position at (0,2) line
	cursorPosition.x = 0;
	cursorPosition.y = 2;
	console_set_cursor_pos( cursorPosition );

	// Print panic message into serial port and on console
	char 			buffer[MAX_PANIC_MESSAGE_SIZE];
	char* 			pPanicMessage = buffer;
	if ( pFormat && pFormat[0] != '\0' )
	{
		va_list 		params;
		va_start( params, pFormat );	
		memset( buffer, '\0', MAX_PANIC_MESSAGE_SIZE );
		vsprintf( buffer, pFormat, params );	
		va_end( params );
	}
	else
	{
		pPanicMessage = "Unknown error";
	}
	
	debugf( "[panic] %s\n", pPanicMessage );	
	printf( "%s", pPanicMessage );

	// Halt the CPU
	hal_panic();
}
