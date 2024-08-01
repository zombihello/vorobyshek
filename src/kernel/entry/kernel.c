#include "graphics/console.h"
#include "utilities/printf.h"

void kernel_main()
{	
	console_init();
	console_set_color( VGA_COLOR_RED, VGA_COLOR_WHITE );
	printf( "Hello World\n" );
}
