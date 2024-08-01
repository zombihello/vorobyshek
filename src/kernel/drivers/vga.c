#include "drivers/vga.h"

#define VGA_WIDTH 		80
#define VGA_HEIGHT 		25
#define VGA_MEMORY 		( ( uint16_t* )0xB8000 )

// -------------------
// GLOBALS
// -------------------
vgaFrameBuffer_t 		g_VGAFrameBuffer = 
{ 
	.width 		= VGA_WIDTH,
	.height 	= VGA_HEIGHT,
	.address 	= VGA_MEMORY
};
