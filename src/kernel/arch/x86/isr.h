#ifndef ARCH_X86_ISR_H
#define ARCH_X86_ISR_H

#include <stdint.h>

// Interrupt Service Routine's registers
// NOTE: In the reverse order they are pushed
#ifdef CPU_ARCH_I686
	typedef struct
	{
		uint32_t 		ds; 											// Data segment pushed by us
		uint32_t 		edi, esi, ebp, useless, ebx, edx, ecx, eax; 	// Pusha
		uint32_t 		interrupt, error; 								// We push interrupt, error is pushed automatically (or our dummy)
		uint32_t 		eip, cs, eflags, esp, ss; 						// Pushed automatically by CPU
	} isrRegisters_t;
#elif defined( CPU_ARCH_AMD64 )
	typedef struct
	{
		uint64_t 		ds; 																		// Data segment push by us
		uint64_t 		rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15; 	// Pushed all registers
		uint64_t 		interrupt, error; 															// We push interrupt, error is pushed automatically (or our dummy)
		uint64_t 		rip, cs, rflags, rsp, ss; 													// Pushed automatically by CPU
	} isrRegisters_t;
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64

// Interrupt Service Routine handler
typedef isrRegisters_t* ( *isrHandlerFn_t )( isrRegisters_t* pISRRegisters );

// Initialize Interrupt Service Routine
void x86_isr_init();

// Register handler of ISR
void x86_isr_register_handler( int interrupt, isrHandlerFn_t pHandlerFn );

#endif // !ARCH_X86_ISR_H
