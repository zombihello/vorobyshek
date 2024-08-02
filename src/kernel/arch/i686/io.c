#include "arch/i686/io.h"

// Unused port to IO wait
#define UNUSED_IO_PORT 			0x80

uint8_t i686_inportb( uint16_t port )
{
	uint8_t 	data;
	__asm__ __volatile__( "inb %1, %0" : "=a"( data ) : "dN"( port ) );
	return data;
}

uint16_t i686_inportw( uint16_t port )
{
	uint16_t 	data;
	__asm__ __volatile__( "in %%dx, %%ax" : "=a"( data ) : "d"( port ) );
	return data;
}

uint32_t i686_inportl( uint16_t port )
{
	uint32_t 	data;
	__asm__ __volatile__( "inl %%dx, %%eax" : "=a"( data ) : "d"( port ) );
	return data;
}

void i686_outportb( uint16_t port, uint8_t data )
{
	__asm__ __volatile__( "outb %1, %0" : : "dN"( port ), "a"( data ) );
}

void i686_outportw( uint16_t port, uint16_t data )
{
	__asm__ __volatile__( "out %%ax, %%dx" : : "a"( data ), "d"( port ) );
}

void i686_outportl( uint16_t port, uint32_t data )
{
	__asm__ __volatile__( "outl %%eax, %%dx" : : "d"( port ), "a"( data ) );
}

void i686_iowait()
{
	i686_outportb( UNUSED_IO_PORT, 0x00 );
}
