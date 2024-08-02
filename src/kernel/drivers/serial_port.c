#include <stdarg.h>

#include "drivers/serial_port.h"
#include "system/hal.h"
#include "utils/printf.h"

static void serial_enable_port( uint16_t port )
{
	hal_outportb( port + 1, 0x00 ); 	// Disable all interrupts
	hal_outportb( port + 3, 0x80 ); 	// Enable DLAB (set boud rate divisor)
	hal_outportb( port + 0, 0x03 ); 	// Set divisor to 3 (low byte) 38400 baud
	hal_outportb( port + 1, 0x00 ); 	// 					(high byte)
	hal_outportb( port + 3, 0x03 ); 	// 8 bits, no parity, one stop bit
	hal_outportb( port + 2, 0xC7 ); 	// Enable FIFO, clear them, with 14-byte threshold
	hal_outportb( port + 4, 0x0B ); 	// IRQs enabled, RTS/DSR set
	debugf( "[serial_port] Enabled serial port 0x%X\n", port );
}

void serial_init()
{
	debugf( "[serial_port] Initialize serial ports\n" );
	serial_enable_port( COM1_PORT );
	hal_outportb( COM1_PORT + 1, 0x01 ); 		// Enable interrupt of recevied data
}

void serial_send( uint16_t port, uint8_t data )
{
	while ( !( serial_recv_status( port ) & 0x20 ) );
	hal_outportb( port, data );
}

uint8_t serial_recv_async( uint16_t port )
{
	return hal_inportb( port );
}

uint8_t serial_recv( uint16_t port )
{
	while ( !( serial_recv_status( port ) & 0x01 ) );
	return hal_inportb( port );
}

uint8_t serial_recv_status( uint16_t port )
{
	return hal_inportb( port + 5 );
}

static void com1_print_debug( char c, void* pUserData )
{
	serial_send( COM1_PORT, c );
}

int debugf( const char *pFormat, ... )
{
	va_list 	params;
	va_start( params, pFormat );
	int 		result = vfctprintf( com1_print_debug, NULL, pFormat, params );
	va_end( params );
	return result;
}
