#include "graphics/console.h"

void KernelMain() 
{
	ConsoleInit();
	ConsoleSetColor( VGA_COLOR_RED, VGA_COLOR_WHITE );
	ConsoleWrite( "Hello World!" );
}
