#ifndef ARCH_I686_IO_H
#define ARCH_I686_IO_H

#include <stdint.h>

// Read data from port
uint8_t 	i686_inportb( uint16_t port );
uint16_t 	i686_inportw( uint16_t port );
uint32_t 	i686_inportl( uint16_t port );

// Write data to port
void 		i686_outportb( uint16_t port, uint8_t data );
void 		i686_outportw( uint16_t port, uint16_t data );
void 		i686_outportl( uint16_t port, uint32_t data );

// IO wait
void 		i686_iowait();

#endif // !ARCH_I686_IO_H
