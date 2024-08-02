#include <stdint.h>

#include "drivers/serial_port.h"
#include "arch/i686/gdt.h"

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
	uint16_t 		limit; 			// The size of the table in bytes subtracted by 1 
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

//
// Helper macros
//
#define GDT_LIMIT_LOW( limit ) 					( limit & 0xFFFF )
#define GDT_BASE_LOW( base ) 					( base & 0xFFFF )
#define GDT_BASE_MIDDLE( base ) 				( ( base >> 16 ) & 0xFF )
#define GDT_FLAGS_LIMIT_HIGH( limit, flags ) 	( ( ( limit >> 16 ) & 0xF ) | ( flags & 0xF0 ) )
#define GDT_BASE_HIGH( base ) 					( ( base >> 24 ) & 0xFF )
#define GDT_MAKE_ENTRY( base, limit, access, flags ) \
	{ \
		GDT_LIMIT_LOW( limit ), \
		GDT_BASE_LOW( base ), \
		GDT_BASE_MIDDLE( base ), \
		access, \
		GDT_FLAGS_LIMIT_HIGH( limit, flags ), \
		GDT_BASE_HIGH( base ) \
	}

// Global Descriptor Table
static gdtEntry_t 		s_GDT[] =
{
	// NULL descriptor (Offset 0x00)
	GDT_MAKE_ENTRY( 0, 0, 0, 0 ),

	// Kernel 32-bit code segment (Offset 0x08)
	GDT_MAKE_ENTRY( 
			0,
			0xFFFFF,
			GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
			GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K ),

	// Kernel 32-bit data segment (Offset 0x10)
	GDT_MAKE_ENTRY(
			0,
			0xFFFFF,
			GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
			GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K )
};

// GDT descriptor
static gdtDescriptor_t 		s_GDTDescriptor = 
{
	.limit 	= sizeof( s_GDT ) - 1,
	.pPtr 	= s_GDT
};

// Initialize Global Descriptor Table
void i686_gdt_init()
{
	debugf( "[gdt] Initialize Global Descriptor Table\n" );
	__asm__ __volatile__(
			"lgdt %0\n\t" 							// Load GDT
			"ljmp %1, $.reload_cs\n\t" 				// Do far jump to reload CS. At %1 we have kernel code segment (offset to GDT entry. [0] = 0, [1] = 0x08, [2] = 0x10, etc)
			".reload_cs:\n\t" 						// When we jump to here CS will be reloaded
				"movl %2, %%eax\n\t" 				// Push segment selector (kernel data) to EAX
				"movl %%eax, %%ds\n\t" 				// Set segment register DS
				"movl %%eax, %%es\n\t" 				// Set segment register ES
				"movl %%eax, %%fs\n\t" 				// Set segment register FS
				"movl %%eax, %%gs\n\t" 				// Set segment register GS
				"movl %%eax, %%ss\n\t" 				// Set segment register SS
			:
			: "m"( s_GDTDescriptor ), "i"( GDT_KERNEL_CODE_SEGMENT ), "i"( GDT_KERNEL_DATA_SEGMENT )
			: "memory"
			);
}
