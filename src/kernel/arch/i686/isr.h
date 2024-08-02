#ifndef ARCH_I686_ISR_H
#define ARCH_I686_ISR_H

#include <stdint.h>

// Interrupt Service Routine's registers
// NOTE: In the reverse order they are pushed
typedef struct
{
	uint32_t 		ds; 											// Data segment pushed by us
	uint32_t 		edi, esi, ebp, useless, ebx, edx, ecx, eax; 	// Pusha
	uint32_t 		interrupt, error; 								// We push interrupt, errir is pushed automatically (or out dummy)
	uint32_t 		eip, cs, eflags, esp, ss; 						// Pushed automatically by CPU
} __attribute__( ( packed ) ) isrRegisters_t;

// Interrupt Service Routine handler
typedef void (*isrHandlerFn_t)( isrRegisters_t* pISRRegisters );

// Initialize Interrupt Service Routine
void i686_isr_init();

// Register handler of ISR
void i686_isr_register_handler( int interrupt, isrHandlerFn_t pHandlerFn );

#endif // !ARCH_I686_ISR_H
