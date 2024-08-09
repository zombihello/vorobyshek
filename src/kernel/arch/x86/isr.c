#include <stddef.h>

#include "arch/x86/isr.h"
#include "arch/x86/idt.h"
#include "system/panic.h"
#include "drivers/serial_port.h"

// -------------
// GLOBALS
// -------------
static isrHandlerFn_t 		s_pISRHandlers[IDT_MAX_ENTRIES];
static const char* 			s_pExceptionsMsg[] = 
{
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "<Unknown>",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "<Unknown>",
    "<Unknown>",
    "<Unknown>",
    "<Unknown>",
    "<Unknown>",
    "<Unknown>",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "<Unknown>"
};

void x86_isr_gates_init();
void x86_isr_init()
{
	debugf( "[isr] Initialize Interrupt Service Routine\n" );
	x86_isr_gates_init();
	for ( int index = 0; index < IDT_MAX_ENTRIES; ++index )
	{
		x86_idt_enable_gate( index );
	}
}

void x86_isr_register_handler( int interrupt, isrHandlerFn_t pHandlerFn )
{
	s_pISRHandlers[interrupt] = pHandlerFn;
	x86_idt_enable_gate( interrupt );
}

isrRegisters_t* x86_isr_handler( isrRegisters_t* pISRRegisters )
{
	// Call ISR handler if we have it
	if ( s_pISRHandlers[pISRRegisters->interrupt] != NULL )
	{
		return s_pISRHandlers[pISRRegisters->interrupt]( pISRRegisters );
	}
	// If we have interrupt number more or equal to 32 is is unhandled interrupt
	else if ( pISRRegisters->interrupt >= 32 )
	{
		debugf( "[isr] Unhandled interrupt 0x%X\n", pISRRegisters->interrupt );
	}
	// Otherwise it is critical exception
	else
	{
		panic( 	"Unhandled exception (%s)\n\n"
				"Registers:\n"
#ifdef CPU_ARCH_I686
				"\tEAX=0x%X  EBX=0x%X  ECX=0x%X  EDX=0x%X  ESI=0x%X  EDI=0x%X\n"
				"\tESP=0x%X  EBP=0x%X  EIP=0x%X  EFLAGS=0x%X  CS=0x%X  DS=0x%X  SS=0x%X\n"
				"\tINTERRUPT=0x%X ERROR_CODE=0x%X",
				s_pExceptionsMsg[pISRRegisters->interrupt],
				pISRRegisters->eax, pISRRegisters->ebx, pISRRegisters->ecx, pISRRegisters->edx, pISRRegisters->esi, pISRRegisters->edi,
				pISRRegisters->esp, pISRRegisters->ebp, pISRRegisters->eip, pISRRegisters->eflags, pISRRegisters->cs, pISRRegisters->ds, pISRRegisters->ss,
				pISRRegisters->interrupt, pISRRegisters->error
#elif defined( CPU_ARCH_AMD64 )
				"\tRAX=0x%lX  RBX=0x%lX  RCX=0x%lX  RDX=0x%lX  RSI=0x%lX    RDI=0x%lX\n"
				"\tRBP=0x%lX  R8=0x%lX   R9=0x%lX   R10=0x%lX  R11=0x%lX    R12=0x%lX\n"
				"\tR13=0x%lX  R14=0x%lX  R15=0x%lX  RIP=0x%lX  RFLAGS=0x%lX RSP=0x%lX\n"
				"\tCS=0x%lX   DS=0x%lX   SS=0x%lX\n"
				"\tINTERRUPT=0x%lX      ERROR_CODE=0x%lX",
				s_pExceptionsMsg[pISRRegisters->interrupt],
				pISRRegisters->rax, pISRRegisters->rbx, pISRRegisters->rcx, pISRRegisters->rdx, pISRRegisters->rsi, pISRRegisters->rdi,
				pISRRegisters->rbp, pISRRegisters->r8, pISRRegisters->r9, pISRRegisters->r10, pISRRegisters->r11, pISRRegisters->r12,
				pISRRegisters->r13, pISRRegisters->r14, pISRRegisters->r15, pISRRegisters->rip, pISRRegisters->rflags, pISRRegisters->rsp,
				pISRRegisters->cs, pISRRegisters->ds, pISRRegisters->ss,
				pISRRegisters->interrupt, pISRRegisters->error
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
				);
	}

	// Return pointer to stack
	return pISRRegisters;
}
