#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdint.h>

// Macro to make VGA color
#define VGA_MAKE_COLOR( textColor, backgroundColor ) 	( textColor | backgroundColor << 4 )

// Macro to make VGA character with color
#define VGA_MAKE_CHAR( ch, color ) 						( ( uint16_t )ch | ( uint16_t )color << 8 )

// Hardware text mode color constants
typedef enum 
{
	VGA_COLOR_BLACK 		= 0,
	VGA_COLOR_BLUE 			= 1,
	VGA_COLOR_GREEN 		= 2,
	VGA_COLOR_CYAN 			= 3,
	VGA_COLOR_RED 			= 4,
	VGA_COLOR_MAGENTA 		= 5,
	VGA_COLOR_BROWN 		= 6,
	VGA_COLOR_LIGHT_GREY 	= 7,
	VGA_COLOR_DARK_GREY 	= 8,
	VGA_COLOR_LIGHT_BLUE 	= 9,
	VGA_COLOR_LIGHT_GREEN 	= 10,
	VGA_COLOR_LIGHT_CYAN 	= 11,
	VGA_COLOR_LIGHT_RED 	= 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN 	= 14,
	VGA_COLOR_WHITE 		= 15
} vgaColor_t;

// Hardware text mode information
typedef struct
{
	size_t 		width;
	size_t 		height;
	uint16_t* 	address;
} vgaFrameBuffer_t;
extern vgaFrameBuffer_t 	g_VGAFrameBuffer;

#endif // !VGA_H
