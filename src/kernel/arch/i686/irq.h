#ifndef ARCH_I686_IRQ_H
#define ARCH_I686_IRQ_H

#include "arch/i686/isr.h"
#include "arch/i686/pic.h"

// Initialize Interrupt Request
void 	i686_irq_init();

// Register IRQ handler
void 	i686_irq_register_handler( int irq, isrHandlerFn_t pHandlerFn );

#endif // !ARCH_I686_IRQ_H
