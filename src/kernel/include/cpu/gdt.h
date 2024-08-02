#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_KERNEL_CODE_SEGMENT 		0x08
#define GDT_KERNEL_DATA_SEGMENT 		0x10

// Entry of Global Descriptor Table
typedef struct
{
	uint16_t 		limitLow; 			// Limit (bits 0-15)
	uint16_t 		baseLow; 			// Base (bits 0-15)
	uint8_t 		baseMiddle; 		// Base (bits 16-23)
	uint8_t 		access; 			// Access (bits 0-7)
	uint8_t 		flagsLimitHigh; 	// Limit (bits 16-19) and flags (bits 0-3)
	uint8_t 		baseHigh; 			// Base (bits 24-31)
} __attribute__( ( packed ) ) gdtEntry_t;

// GDT descriptor
typedef struct
{
	uint16_t 		limit; 			// Size of global descriptor table minus 1
	gdtEntry_t* 	pPtr; 			// Address of GDT
} __attribute__( ( packed ) ) gdtDescriptor_t;

// GDT access flags
typedef enum
{
	GDT_ACCESS_CODE_READABLE 				= 0x02,
	GDT_ACCESS_DATA_WRITEABLE 				= 0x02,

	GDT_ACCESS_CODE_CONFORMING 				= 0x04,
	GDT_ACCESS_DATA_DIRECTION_NORMAL 		= 0x00,
	GDT_ACCESS_DATA_DIRECTION_DOWN 			= 0x04,

	GDT_ACCESS_DATA_SEGMENT 				= 0x10,
	GDT_ACCESS_CODE_SEGMENT 				= 0x18,

	GDT_ACCESS_DESCRIPTOR_TSS 				= 0x00,

	GDT_ACCESS_RING0 						= 0x00,
	GDT_ACCESS_RING1 						= 0x20,
	GDT_ACCESS_RING2 						= 0x40,
	GDT_ACCESS_RING3 						= 0x60,

	GDT_ACCESS_PRESENT 						= 0x80
} gdtAccess_t;

// GDT flags
typedef enum
{
	GDT_FLAG_64BIT 							= 0x20,
	GDT_FLAG_32BIT 							= 0x40,
	GDT_FLAG_16BIT 							= 0x00,

	GDT_FLAG_GRANULARITY_1B 				= 0x00,
	GDT_FLAG_GRANULARITY_4K 				= 0x80
} gdtFlags_t;

// Initialize global descriptor table
void 	gdt_init();

#endif // !GDT_H
