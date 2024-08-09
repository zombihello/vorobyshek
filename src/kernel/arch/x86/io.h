#ifndef ARCH_X86_IO_H
#define ARCH_X86_IO_H

#include <stdint.h>

// Read data from port
uint8_t 	x86_inportb( uint16_t port );
uint16_t 	x86_inportw( uint16_t port );
uint32_t 	x86_inportl( uint16_t port );

// Write data to port
void 		x86_outportb( uint16_t port, uint8_t data );
void 		x86_outportw( uint16_t port, uint16_t data );
void 		x86_outportl( uint16_t port, uint32_t data );

// IO wait
void 		x86_iowait();

#endif // !ARCH_X86_IO_H
