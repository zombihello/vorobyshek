#include <stddef.h>
#include "arch/i686/irq.h"
#include "arch/i686/cpu.h"
#include "drivers/serial_port.h"

// IRQs list:
// ------------------------------------------------------
// PIC1
// 0: 		System timer
// 1: 		Keyboard
// 2: 		Cascaded signals from IRQs 8-15
// 3: 		Serial port controller for COM2/COM4
// 4: 		Serial port controller for COM1/COM3
// 5: 		Parallel port 2 / Sound card
// 6: 		Floppy disk controller
// 7: 		Parallel port 1
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PIC2
// 8: 		Real-time clock
// 9: 		ACPI (when not in use, redirected to IRQ2)
// 10: 		Reserved
// 11: 		Reserved
// 12: 		PS/2 mouse
// 13: 		FPU / Coprocessor / Intel-processor (math coprocessor)
// 14: 		Primary ATA channel
// 15: 		Secondary ATA channel

// IRQ handlers
static isrHandlerFn_t 			s_pIRQHandlers[PIC_MAX_INTERRUPTS];

void i686_irq_handler( isrRegisters_t* pISRRegisters )
{
	// Handle IRQ
	// NOTE for the future:
	// To proper handle of IRQ1 (keyboard PS2), otherwise IRQ1 will be trigger one time
	// https://github.com/dreamportdev/Osdev-Notes/tree/master/02_Architecture
	// uint8_t scan = i686_inportb( 0x60 );
	// uint8_t i = i686_inportb( 0x61 );
	// i686_outportb( 0x61, i|0x80 );
	// i686_outportb( 0x61, i );

	int 		irq 	= pISRRegisters->interrupt - PIC_REMAP_OFFSET;
	uint8_t 	picISR 	= i686_pic_get_isr();
	uint8_t 	picIRR 	= i686_pic_get_irr();
	
	// Spurious IRQs handle
	if ( !( picISR & ( 1 << irq ) ) )
	{
		// If it was the slave PIC we have to send EOI to the master PIC
		if ( irq > 7 )
		{
			i686_pic_send_eoi( irq );
		}

		debugf( "[irq] Spurious IRQ 0x%X (ISR=0x%X IRR=0x%X)\n", irq, picISR, picIRR );
		return;
	}

	if ( s_pIRQHandlers[irq] != NULL )
	{
		s_pIRQHandlers[irq]( pISRRegisters );
	}
	else
	{
		debugf( "[irq] Unhandled IRQ 0x%X (ISR=0x%X IRR=0x%X)\n", irq, picISR, picIRR );
	}

	// Send EOI to the PIC controllers
	i686_pic_send_eoi( irq );
}

void i686_irq_init()
{
	// Initialize the PIC controllers
	debugf( "[irq] Initialize IRQs\n" );
	i686_pic_configure( PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8 );

	// Register ISR handlers for each of the 16 IRQ lines
	for ( int index = 0; index < PIC_MAX_INTERRUPTS; ++index )
	{
		i686_isr_register_handler( PIC_REMAP_OFFSET + index, i686_irq_handler );
	}

	// Enable interrupts
	i686_enable_interrupts();
}

void i686_irq_register_handler( int irq, isrHandlerFn_t pHandlerFn )
{
	s_pIRQHandlers[irq] = pHandlerFn;
}
