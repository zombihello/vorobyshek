#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void kernel_main() 
{
	uint16_t* 	pTerminalBuffer = ( uint16_t* )0xB8000;
	pTerminalBuffer[0] = 'A' | 2 << 8;
}
