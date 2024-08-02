#include "arch/i686/idt.h"
#include "drivers/serial_port.h"

// Entry of Interrupt Description Table
typedef struct
{
	uint16_t 		isrLow; 			// The lower 16 bit of the ISR's address
	uint16_t 		segmenSelector; 	// The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t 		reserved; 			// Set to zero
	uint8_t 		flags; 				// Type and attributes
	uint16_t 		isrHeigh; 			// The higher 16 bits of the ISR's address
} __attribute__( ( packed ) ) idtEntry_t;

// Descriptor of Interrupt Descriptor Table
typedef struct
{
	uint16_t 		limit; 			// The sie of the table in bytes subtracted by 1
	idtEntry_t* 	pPtr; 			// Address of IDT
} __attribute__( ( packed ) ) idtDescriptor_t;

// Interrupt Descriptor Table
idtEntry_t 			s_IDT[IDT_MAX_ENTRIES];

// IDT descriptor
idtDescriptor_t 	s_IDTDescriptor =
{
	.limit 	= sizeof( s_IDT ) - 1,
	.pPtr 	= s_IDT
};

void i686_idt_set_gate( int interrupt, void* pFunc, uint16_t segmentDescriptor, uint8_t flags )
{
	s_IDT[interrupt].isrLow 			= ( ( uint32_t )pFunc ) & 0xFFFF;
	s_IDT[interrupt].segmenSelector 	= segmentDescriptor;
	s_IDT[interrupt].reserved 			= 0;
	s_IDT[interrupt].flags 				= flags;
	s_IDT[interrupt].isrHeigh 			= ( ( uint32_t )pFunc >> 16 ) & 0xFFFF;
}

void i686_idt_enable_gate( int interrupt )
{
	s_IDT[interrupt].flags |= IDT_FLAG_PRESENT;
}

void i686_idt_disable_gate( int interrupt )
{
	s_IDT[interrupt].flags &= ~IDT_FLAG_PRESENT;
}

void i686_idt_init()
{
	debugf( "[idt] Initialize Interrupt Descriptor Table\n" );
	__asm__ __volatile__(
			"lidt %0\n\t" 		// Load IDT
			:
			: "m"( s_IDTDescriptor )
			: "memory"
			);
}
