#include "cpu/cpu.h"

//
// Handling ports
//

uint8_t inportb( uint16_t port )
{
	uint8_t 	data;
	__asm__ __volatile__( "inb %1, %0" : "=a"( data ) : "dN"( port ) );
	return data;
}

uint16_t inportw( uint16_t port )
{
	uint16_t 	data;
	__asm__ __volatile__( "in %%dx, %%ax" : "=a"( data ) : "d"( port ) );
	return data;
}

uint32_t inportl( uint16_t port )
{
	uint32_t 	data;
	__asm__ __volatile__( "inl %%dx, %%eax" : "=a"( data ) : "d"( port ) );
	return data;
}

void outportb( uint16_t port, uint8_t data )
{
	__asm__ __volatile__( "outb %1, %0" : : "dN"( port ), "a"( data ) );
}

void outportw( uint16_t port, uint16_t data )
{
	__asm__ __volatile__( "out %%ax, %%dx" : : "a"( data ), "d"( port ) );
}

void outportl( uint16_t port, uint32_t data )
{
	__asm__ __volatile__( "outl %%eax, %%dx" : : "d"( port ), "a"( data ) );
}

//
// Generic functions
//

void panic()
{
	__asm__ __volatile__
	( 
		"cli\n\t"
		"hlt\n\t"
	);
}
