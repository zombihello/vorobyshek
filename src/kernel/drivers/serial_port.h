#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdint.h>

// COM ports
#define COM1_PORT 		0x3F8
#define COM2_PORT 		0x2F8
#define COM3_PORT 		0x3E8
#define COM4_PORT 		0x2E8
#define COM5_PORT 		0x5F8
#define COM6_PORT 		0x4F8
#define COM7_PORT 		0x5E8
#define COM8_PORT 		0x4E8

// Initialaze serial ports
void 		serial_init();

// Send data to serial port
void 		serial_send( uint16_t port, uint8_t data );

// Recive data from serial port (async and sync versions)
uint8_t 	serial_recv_async( uint16_t port );
uint8_t 	serial_recv( uint16_t port );

// Revice line status register on serial port
uint8_t 	serial_recv_status( uint16_t port );

// Print debug message into COM1 port
int debugf( const char* pFormat, ... );

#endif // !SERIAL_PORT_H
