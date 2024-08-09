#ifndef ARCH_X86_IRQ_H
#define ARCH_X86_IRQ_H

#include "arch/x86/isr.h"
#include "arch/x86/pic.h"

// Initialize Interrupt Request
void 	x86_irq_init();

// Register IRQ handler
void 	x86_irq_register_handler( int irq, isrHandlerFn_t pHandlerFn );

#endif // !ARCH_X86_IRQ_H
