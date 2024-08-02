#include "drivers/serial_port.h"
#include "cpu/gdt.h"

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
void gdt_init()
{
	debugf( "[gdt]: Initialize Global Descriptor Table\n" );
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
