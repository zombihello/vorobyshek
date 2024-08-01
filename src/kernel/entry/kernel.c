#include "graphics/console.h"
#include "drivers/serial_port.h"
#include "utilities/printf.h"

void kernel_main()
{	
	serial_init();
	console_init();
	console_set_color( VGA_COLOR_RED, VGA_COLOR_WHITE );
	printf( "Hello World\n" );
	debugf( "Serial Port HELLO!\n" );
}
