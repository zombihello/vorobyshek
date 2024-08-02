#include <stdint.h>
#include "graphics/console.h"

// ------------------
// GLOBAL
// ------------------
static consoleCursorPos_t 		s_ConsoleCursorPos 	= { .x = 0, .y = 0 };
static uint8_t 					s_ConsoleColor 		= VGA_MAKE_COLOR( VGA_COLOR_WHITE, VGA_COLOR_BLACK );

void console_init()
{
	console_clear();
}

void console_clear()
{
	s_ConsoleCursorPos.x = 0;
	s_ConsoleCursorPos.y = 0;
	for ( size_t y = 0; y < g_VGAFrameBuffer.height; ++y )
	{
		for ( size_t x = 0; x < g_VGAFrameBuffer.width; ++x )
		{
			g_VGAFrameBuffer.address[y * g_VGAFrameBuffer.width + x] = VGA_MAKE_CHAR( ' ', s_ConsoleColor );
		}
	}
}

void console_clear_line( size_t line )
{
	for ( size_t x = 0; x < g_VGAFrameBuffer.width; ++x )
	{
		g_VGAFrameBuffer.address[line * g_VGAFrameBuffer.width +x] = VGA_MAKE_CHAR( ' ', s_ConsoleColor );
	}
}

void console_set_color( vgaColor_t textColor, vgaColor_t backgroundColor )
{
	s_ConsoleColor = VGA_MAKE_COLOR( textColor, backgroundColor );
}

void console_set_cursor_pos( consoleCursorPos_t cursorPosition )
{
	s_ConsoleCursorPos.x = cursorPosition.x % g_VGAFrameBuffer.width;
	s_ConsoleCursorPos.y = cursorPosition.y % g_VGAFrameBuffer.height;
}

consoleCursorPos_t console_get_cursor_pos()
{
	return s_ConsoleCursorPos;
}

void console_put_char_at( char c, vgaColor_t textColor, vgaColor_t backgroundColor, size_t x, size_t y )
{
	g_VGAFrameBuffer.address[y * g_VGAFrameBuffer.width + x] = VGA_MAKE_CHAR( c, VGA_MAKE_COLOR( textColor, backgroundColor ) );
}

static void console_scroll( size_t line )
{
	uint16_t* 	pCurAddress 	= g_VGAFrameBuffer.address + line * g_VGAFrameBuffer.width;
	uint16_t* 	pEndAddress 	= pCurAddress + g_VGAFrameBuffer.width;
	while( pCurAddress != pEndAddress )
	{
		uint16_t 	data = *pCurAddress;
		*( pCurAddress - g_VGAFrameBuffer.width ) = data;
		++pCurAddress;
	}
}

static void console_delete_last_line()
{
	uint16_t* 	pCurAddress = g_VGAFrameBuffer.address + ( g_VGAFrameBuffer.height-1 ) * g_VGAFrameBuffer.width;
	for ( size_t x = 0; x < g_VGAFrameBuffer.width; ++x, ++pCurAddress )
	{
		*pCurAddress = VGA_MAKE_CHAR( ' ', s_ConsoleColor );
	}
}

void console_put_char( char c )
{
	if ( s_ConsoleCursorPos.x >= g_VGAFrameBuffer.width )
	{
		s_ConsoleCursorPos.x = 0;
		++s_ConsoleCursorPos.y;
	}

	if ( s_ConsoleCursorPos.y >= g_VGAFrameBuffer.height )
	{
		for ( size_t line = 1; line <= g_VGAFrameBuffer.height - 1; ++line )
		{
			console_scroll( line );
		}
		
		console_delete_last_line();
		s_ConsoleCursorPos.y = g_VGAFrameBuffer.height-1;
	}

	switch ( c )
	{
	case '\n':
		++s_ConsoleCursorPos.y;
		s_ConsoleCursorPos.x = 0;
		break;

	case '\t':
		for ( int index = 0; index < 4; index++ )
		{
			console_put_char( ' ' );
		}
		break;

	default:
		g_VGAFrameBuffer.address[s_ConsoleCursorPos.y * g_VGAFrameBuffer.width + s_ConsoleCursorPos.x] = VGA_MAKE_CHAR( c, s_ConsoleColor );
		++s_ConsoleCursorPos.x;
		break;
	}
}
