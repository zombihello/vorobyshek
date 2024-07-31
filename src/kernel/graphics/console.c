#include <stdint.h>
#include "graphics/console.h"

// ------------------
// GLOBAL
// ------------------
static size_t 		s_ConsoleRow 		= 0;
static size_t 		s_ConsoleColumn 	= 0;
static uint8_t 		s_ConsoleColor 		= VGA_MAKE_COLOR( VGA_COLOR_WHITE, VGA_COLOR_BLACK );
static uint16_t* 	s_pConsoleBuffer 	= VGA_MEMORY;

void ConsoleInit()
{
	ConsoleClear();
}

void ConsoleClear()
{
	s_ConsoleRow 		= 0;
	s_ConsoleColumn 	= 0;
	for ( size_t y = 0; y < VGA_HEIGHT; ++y )
	{
		for ( size_t x = 0; x < VGA_WIDTH; ++x )
		{
			s_pConsoleBuffer[y * VGA_WIDTH + x] = VGA_MAKE_CHAR( ' ', s_ConsoleColor );
		}
	}
}

void ConsoleSetColor( vgaColor_t textColor, vgaColor_t backgroundColor )
{
	s_ConsoleColor = VGA_MAKE_COLOR( textColor, backgroundColor );
}

void ConsolePutCharAt( char c, vgaColor_t textColor, vgaColor_t backgroundColor, size_t x, size_t y )
{
	s_pConsoleBuffer[y * VGA_WIDTH + x] = VGA_MAKE_CHAR( c, VGA_MAKE_COLOR( textColor, backgroundColor ) );
}

void ConsolePutChar( char c )
{
	s_pConsoleBuffer[s_ConsoleRow * VGA_WIDTH + s_ConsoleColumn] = VGA_MAKE_CHAR( c, s_ConsoleColor );
	if ( ++s_ConsoleColumn >= VGA_WIDTH )
	{
		s_ConsoleColumn = 0;
		if ( ++s_ConsoleRow >= VGA_HEIGHT )
		{
			s_ConsoleRow = 0;
		}
	}
}

void ConsoleWrite( const char *pStr )
{
	for ( size_t index = 0; pStr[index] != '\0'; ++index )
	{
		ConsolePutChar( pStr[index] );
	}
}
