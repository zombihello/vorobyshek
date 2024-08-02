#include <stdarg.h>

#include "drivers/serial_port.h"
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
	const size_t 			centerHeight 		= g_VGAFrameBuffer.height / 2;
	consoleCursorPos_t 		cursorPosition 		= 
	{ 
		.x = centerWidth - 4,
		.y = centerHeight
	};
	
	console_set_color( VGA_COLOR_WHITE, VGA_COLOR_BLUE );
	console_clear();
	console_set_color( VGA_COLOR_BLUE, VGA_COLOR_WHITE );	
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
	
	printf( "  PANIC!  \n" );
	cursorPosition 		= console_get_cursor_pos();
	cursorPosition.x 	= centerWidth - strlen( pPanicMessage ) / 2;
	console_set_cursor_pos( cursorPosition );
	printf( "%s", pPanicMessage );

	// Halt CPU
	__asm__ __volatile__
	( 
		"cli\n\t"
		"hlt\n\t"
	);
}
