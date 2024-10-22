#ifndef HAL_H
#define HAL_H

#include <stdint.h>

// Initialize hardware layer interface
void  		hal_init();

// Read data from port
uint8_t 	hal_inportb( uint16_t port );
uint16_t 	hal_inportw( uint16_t port );
uint32_t 	hal_inportl( uint16_t port );

// Write data to port
void 		hal_outportb( uint16_t port, uint8_t data );
void 		hal_outportw( uint16_t port, uint16_t data );
void 		hal_outportl( uint16_t port, uint32_t data );

// IO wait
void 		hal_iowait();

// Halt the CPU
void 		hal_panic();

// Enable and disable interrupts
void 		hal_enable_interrupts();
void 		hal_disable_interrupts();

#endif // !HAL_H
