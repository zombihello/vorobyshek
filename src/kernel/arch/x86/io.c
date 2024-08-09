#include "arch/x86/io.h"

// Unused port to IO wait
#define UNUSED_IO_PORT 			0x80

uint8_t x86_inportb( uint16_t port )
{
	uint8_t 	data;
	__asm__ __volatile__( "inb %1, %0" : "=a"( data ) : "dN"( port ) );
	return data;
}

uint16_t x86_inportw( uint16_t port )
{
	uint16_t 	data;
	__asm__ __volatile__( "in %%dx, %%ax" : "=a"( data ) : "d"( port ) );
	return data;
}

uint32_t x86_inportl( uint16_t port )
{
	uint32_t 	data;
	__asm__ __volatile__( "inl %%dx, %%eax" : "=a"( data ) : "d"( port ) );
	return data;
}

void x86_outportb( uint16_t port, uint8_t data )
{
	__asm__ __volatile__( "outb %1, %0" : : "dN"( port ), "a"( data ) );
}

void x86_outportw( uint16_t port, uint16_t data )
{
	__asm__ __volatile__( "out %%ax, %%dx" : : "a"( data ), "d"( port ) );
}

void x86_outportl( uint16_t port, uint32_t data )
{
	__asm__ __volatile__( "outl %%eax, %%dx" : : "d"( port ), "a"( data ) );
}

void x86_iowait()
{
	x86_outportb( UNUSED_IO_PORT, 0x00 );
}
