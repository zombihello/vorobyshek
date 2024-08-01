#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// Read data from port
uint8_t 	inportb( uint16_t port );
uint16_t 	inportw( uint16_t port );
uint32_t 	inportl( uint16_t port );

// Write data to port
void 		outportb( uint16_t port, uint8_t data );
void 		outportw( uint16_t port, uint16_t data );
void 		outportl( uint16_t port, uint32_t data );

// Generic functions
void 		panic();

#endif // !CPU_H
