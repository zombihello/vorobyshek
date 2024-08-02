#include <stddef.h>

#include "arch/i686/isr.h"
#include "arch/i686/idt.h"
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

void i686_isr_gates_init();
void i686_isr_init()
{
	debugf( "[isr] Initialize Interrupt Service Routine\n" );
	i686_isr_gates_init();
	for ( int index = 0; index < IDT_MAX_ENTRIES; ++index )
	{
		i686_idt_enable_gate( index );
	}
}

void i686_isr_register_handler( int interrupt, isrHandlerFn_t pHandlerFn )
{
	s_pISRHandlers[interrupt] = pHandlerFn;
	i686_idt_enable_gate( interrupt );
}

void i686_isr_handler( isrRegisters_t* pISRRegisters )
{
	// Call ISR handler if we have it
	if ( s_pISRHandlers[pISRRegisters->interrupt] != NULL )
	{
		s_pISRHandlers[pISRRegisters->interrupt]( pISRRegisters );
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
				"\tEAX=0x%X  EBX=0x%X  ECX=0x%X  EDX=0x%X  ESI=0x%X  EDI=0x%X\n"
				"\tESP=0x%X  EBP=0x%X  EIP=0x%X  EFLAGS=0x%X  CS=0x%X  DS=0x%X  SS=0x%X\n"
				"\tINTERRUPT=0x%X ERROR_CODE=0x%X",
				s_pExceptionsMsg[pISRRegisters->interrupt],
				pISRRegisters->eax, pISRRegisters->ebx, pISRRegisters->ecx, pISRRegisters->edx, pISRRegisters->esi, pISRRegisters->edi,
				pISRRegisters->esp, pISRRegisters->ebp, pISRRegisters->eip, pISRRegisters->eflags, pISRRegisters->cs, pISRRegisters->ds, pISRRegisters->ss,
				pISRRegisters->interrupt, pISRRegisters->error );
	}
}
