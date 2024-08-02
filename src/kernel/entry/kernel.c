#include "drivers/serial_port.h"
#include "system/hal.h"
#include "graphics/console.h"
#include "system/panic.h"
#include "utils/printf.h"

void kernel_main()
{	
	// Initialize the system
	serial_init();
	hal_init();
	console_init();

	// We are done and can print messages
	console_set_color( VGA_COLOR_RED, VGA_COLOR_WHITE );
	printf( "Hello World\n" );
	debugf( "Serial Port HELLO!\n" );
}
