#include "arch/i686/pic.h"
#include "arch/i686/io.h"
#include "drivers/serial_port.h"

// PIC1 and PIC2 ports
#define PIC1_COMMAND_PORT 		0x20
#define PIC1_DATA_PORT 			0x21
#define PIC2_COMMAND_PORT 		0xA0
#define PIC2_DATA_PORT 			0xA1

// Initialization Control Word 1
// -----------------------------
//  0   IC4     if set, the PIC expects to receive ICW4 during initialization
//  1   SGNL    if set, only 1 PIC in the system; if unset, the PIC is cascaded with slave PICs
//              and ICW3 must be sent to controller
//  2   ADI     call address interval, set: 4, not set: 8; ignored on x86, set to 0
//  3   LTIM    if set, operate in level triggered mode; if unset, operate in edge triggered mode
//  4   INIT    set to 1 to initialize PIC
//  5-7         ignored on x86, set to 0
typedef enum
{
	PIC_ICW1_ICW4 			= 0x01, 	// Indicates that ICW4 will be present
	PIC_ICW1_SINGLE 		= 0x02, 	// Single (cascade) mode
	PIC_ICW1_INTERVAL4 		= 0x04, 	// Call address interval 4 (8)
	PIC_ICW1_LEVEL 			= 0x08, 	// Level triggered (edge) mode
	PIC_ICW1_INITIALIZE 	= 0x10 		// Initialization - required!
} picICW1_t;

// Initialization Control Word 4
// -----------------------------
//  0   uPM     if set, PIC is in 80x86 mode; if cleared, in MCS-80/85 mode
//  1   AEOI    if set, on last interrupt acknowledge pulse, controller automatically performs 
//              end of interrupt operation
//  2   M/S     only use if BUF is set; if set, selects buffer master; otherwise, selects buffer slave
//  3   BUF     if set, controller operates in buffered mode
//  4   SFNM    specially fully nested mode; used in systems with large number of cascaded controllers
//  5-7         reserved, set to 0
typedef enum
{
	PIC_ICW4_8086 			= 0x01, 	// 8086/88 (MCS-80/85) mode
	PIC_ICW4_AUTO_EOI 		= 0x02, 	// Auto (normal) EOI
	PIC_ICW4_BUFFER_SLAVE 	= 0x08, 	// Buffered mode/slave
	PIC_ICW4_BUFFER_MASTER 	= 0x0C, 	// Buffered mode/master
	PIC_ICW4_SFNM 			= 0x10 		// Special fully nasted (not)
} picICW4_t;

// PIC commands
typedef enum
{
	PIC_CMD_END_OF_INTERRUPT 		= 0x20, 	// End of interrupt
	PIC_CMD_IRR 					= 0x0A, 	// OCW3 irq ready next CMD read
	PIC_CMD_ISR 					= 0x0B 		// OCW3 ira service next CMD read
} picCmd_t;

// Initialize the PIC controllers, giving them specified vector offset
// rather than 0x08 and 0x70, as configured by default
void i686_pic_configure( uint8_t offsetPIC1, uint8_t offsetPIC2 )
{
	debugf( "[pic] Initialize the PIC controllers (0x%X, 0x%X)\n", offsetPIC1, offsetPIC2 );

	// Initialization Control Word 1
	i686_outportb( PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE ); 	// Starts the initialization sequence (in cascade mode)
	i686_iowait();
	i686_outportb( PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE );
	i686_iowait();

	// Initialization Control Word 2 - the offsets
	i686_outportb( PIC1_DATA_PORT, offsetPIC1 ); 		// Master PIC vector offset
	i686_iowait();
	i686_outportb( PIC2_DATA_PORT, offsetPIC2 ); 		// Slave PIC vector offset
	i686_iowait();

	// Initialization Control Word 3
	i686_outportb( PIC1_DATA_PORT, 0x04 ); 				// Tell PIC1 that it has a slave at IRQ4 (0000 0100)
	i686_iowait();
	i686_outportb( PIC2_DATA_PORT, 0x02 ); 				// Tell PIC2 its cascade identity (0000 0010)
	i686_iowait();

	// Initialization Control Word 4
	i686_outportb( PIC1_DATA_PORT, PIC_ICW4_8086 ); 	// Have the PICs use 8086 mode (and not 8080 mode)
	i686_iowait();
	i686_outportb( PIC2_DATA_PORT, PIC_ICW4_8086 );
	i686_iowait();

	// Disable all PICs interrupts until they are enabled by the device driver
	i686_pic_disable();
}

void i686_pic_send_eoi( int irq )
{
	if ( irq >= 8 )
	{
		i686_outportb( PIC2_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT );
	}
	i686_outportb( PIC1_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT );
}

void i686_pic_disable()
{
	i686_outportb( PIC1_DATA_PORT, 0xFF );
	i686_iowait();
	i686_outportb( PIC2_DATA_PORT, 0xFF );
	i686_iowait();
}

void i686_pic_set_mask( int irq )
{
	uint8_t 	port;
	if ( irq < 8 )
	{
		port = PIC1_DATA_PORT;
	}
	else
	{
		port = PIC2_DATA_PORT;
		irq -= 8;
	}

	uint8_t 	mask = i686_inportb( port );
	i686_outportb( port, mask | ( 1 << irq ) );
}

void i686_pic_clear_mask( int irq )
{
	uint8_t 	port;
	if ( irq < 8 )
	{
		port = PIC1_DATA_PORT;
	}
	else
	{
		port = PIC2_DATA_PORT;
		irq -= 8;
	}

	uint8_t 	mask = i686_inportb( port );
	i686_outportb( port, mask & ~( 1 << irq ) );
}

static uint16_t i686_pic_get_irq_register( int ocw3 )
{
	// OCW3 to PIC CMD to get the register values.
	// PIC2 is chained and repsresents IRQs 8-15.
	// PIC1 is IRQs 0-7, with 2 being the chain
	i686_outportb( PIC1_COMMAND_PORT, ocw3 );
	i686_outportb( PIC2_COMMAND_PORT, ocw3 );
	return ( i686_inportb( PIC2_COMMAND_PORT ) << 8 ) | i686_inportb( PIC1_COMMAND_PORT );
}

uint16_t i686_pic_get_irr()
{
	// Returns the combined value of the cascaded PICs irq request register
	return i686_pic_get_irq_register( PIC_CMD_IRR );
}

uint16_t i686_pic_get_isr()
{
	// Returns the combined value of the cascaded PICs in-service register
	return i686_pic_get_irq_register( PIC_CMD_ISR );
}
