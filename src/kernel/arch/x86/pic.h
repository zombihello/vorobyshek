#ifndef ARCH_X86_PIC_H
#define ARCH_X86_PIC_H

#include <stdint.h>

// PIC remap offset
#define PIC_REMAP_OFFSET 		0x20

// Maximum PIC interrupts
#define PIC_MAX_INTERRUPTS 		16

// Programmable Interrupt Controller (PIC)
// Initialize the PIC controllers
void x86_pic_configure( uint8_t offsetPIC1, uint8_t offsetPIC2 );

// Send to the PIC controllers end of interrupt
void x86_pic_send_eoi( int irq );

// Disable the PIC controllers
void x86_pic_disable();

// Set mask IRQ in the PIC controllers
void x86_pic_set_mask( int irq );

// Clear mask IRQ in the controllers
void x86_pic_clear_mask( int irq );

// Returns the combined value of the cascaded PICs irq request register
uint16_t x86_pic_get_irr();

// Returns the combined value of the cascaded PICs in-service register
uint16_t x86_pic_get_isr();

#endif // !ARCH_X86_PIC_H
