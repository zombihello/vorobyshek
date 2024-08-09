#include "arch/x86/idt.h"
#include "drivers/serial_port.h"

// Entry of Interrupt Description Table
#ifdef CPU_ARCH_I686
	typedef struct
	{
		uint16_t 		isrLow; 			// The lower 16 bit of the ISR's address (0..15)
		uint16_t 		segmenSelector; 	// The GDT segment selector that the CPU will load into CS before calling the ISR
		uint8_t 		reserved; 			// Set to zero
		uint8_t 		flags; 				// Type and attributes
		uint16_t 		isrHeigh; 			// The higher 16 bits of the ISR's address
	} __attribute__( ( packed ) ) idtEntry_t;
#elif defined( CPU_ARCH_AMD64 )
	typedef struct
	{
		uint16_t 		isrLow; 			// The lower 16 bit of the ISR's address
		uint16_t 		segmenSelector; 	// The GDT segment selector that the CPU will load into CS before calling the ISR
		uint8_t 		ist; 				// Bits 0..2 holds Interrupt Stack Table offset, rest of bits zero. The offset into the IST, which is stored in the Task State Segment (if the bits are all set to zero, the IST is not used)
		uint8_t 		flags; 				// Type and attributes
		uint16_t 		isrMiddle; 			// The middle 16 bit of the ISR's address (16..31)
		uint32_t 		isrHeigh; 			// The higher 32 bit of the ISR's address (32..63)
		uint32_t 		reserved; 			// Set to zero
	} __attribute__( ( packed ) ) idtEntry_t;
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64

// Descriptor of Interrupt Descriptor Table
typedef struct
{
	uint16_t 		limit; 			// The sie of the table in bytes subtracted by 1
	idtEntry_t* 	pPtr; 			// Address of IDT
} __attribute__( ( packed ) ) idtDescriptor_t;

// Interrupt Descriptor Table
static idtEntry_t 			s_IDT[IDT_MAX_ENTRIES];

// IDT descriptor
static idtDescriptor_t 		s_IDTDescriptor =
{
	.limit 	= sizeof( s_IDT ) - 1,
	.pPtr 	= s_IDT
};

void x86_idt_set_gate( int interrupt, void* pFunc, uint16_t segmentDescriptor, uint8_t flags )
{
	s_IDT[interrupt].isrLow 			= ( ( uintptr_t )pFunc ) & 0xFFFF;
	s_IDT[interrupt].segmenSelector 	= segmentDescriptor;
	s_IDT[interrupt].reserved 			= 0;
	s_IDT[interrupt].flags 				= flags;

#ifdef CPU_ARCH_I686
	s_IDT[interrupt].isrHeigh 			= ( ( uintptr_t )pFunc >> 16 ) & 0xFFFF;
#elif defined( CPU_ARCH_AMD64 )
	s_IDT[interrupt].isrMiddle 			= ( ( uintptr_t )pFunc >> 16 ) & 0xFFFF;
	s_IDT[interrupt].isrHeigh 			= ( ( uintptr_t )pFunc >> 32 ) & 0xFFFFFFFF;
	s_IDT[interrupt].ist 				= 0;
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void x86_idt_enable_gate( int interrupt )
{
	s_IDT[interrupt].flags |= IDT_FLAG_PRESENT;
}

void x86_idt_disable_gate( int interrupt )
{
	s_IDT[interrupt].flags &= ~IDT_FLAG_PRESENT;
}

void x86_idt_init()
{
	debugf( "[idt] Initialize Interrupt Descriptor Table\n" );
	__asm__ __volatile__(
			"lidt %0\n\t" 		// Load IDT
			:
			: "m"( s_IDTDescriptor )
			: "memory"
			);

}
