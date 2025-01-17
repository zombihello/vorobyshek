#ifndef ARCH_X86_IDT_H
#define ARCH_X86_IDT_H

#include <stdint.h>

// Maximum count of Interrupt Descriptor Table entries
#define IDT_MAX_ENTRIES 			256

// Interrupt Descriptor Table flags
typedef enum
{
	IDT_FLAG_GATE_TASK 					= 0x5,
	IDT_FLAG_GATE_16BIT_INTERRUPT 		= 0x6,
	IDT_FLAG_GATE_16BIT_TRAP 			= 0x7,
	IDT_FLAG_GATE_32_64BIT_INTERRUPT 	= 0xE,
	IDT_FLAG_GATE_32_64BIT_TRAP 		= 0xF,

	IDT_FLAG_RING0 						= 0 << 5,
	IDT_FLAG_RING1 						= 1 << 5,
	IDT_FLAG_RING2 						= 2 << 5,
	IDT_FLAG_RING3 						= 3 << 5,

	IDT_FLAG_PRESENT 					= 0x80
} idtFlags_t;

// Initialize Interrupt Descriptor Table
void x86_idt_init();

// Enable gate of IDT entry
void x86_idt_enable_gate( int interrupt );

// Disable gate of IDT entry
void x86_idt_disable_gate( int interrupt );

// Set gate of IDT entry
void x86_idt_set_gate( int interrupt, void* pFunc, uint16_t segmentDescriptor, uint8_t flags );

#endif // !ARCH_X86_IDT_H
