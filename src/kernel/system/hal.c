#include "system/hal.h"

// Build kernel for i686 architecture
#ifdef CPU_ARCH_I686
	#include "arch/i686/port.h"
	#include "arch/i686/gdt.h"
	#include "arch/i686/idt.h"
	#include "arch/i686/isr.h"
	#include "arch/i686/panic.h"
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686

void hal_init()
{
#ifdef CPU_ARCH_I686
	i686_gdt_init();
	i686_idt_init();
	i686_isr_init();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686
}

uint8_t hal_inportb( uint16_t port )
{
#ifdef CPU_ARCH_I686
	return i686_inportb( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686
}

uint16_t hal_inportw( uint16_t port )
{
#ifdef CPU_ARCH_I686
	return i686_inportw( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686
}

uint32_t hal_inportl( uint16_t port )
{
#ifdef CPU_ARCH_I686
	return i686_inportl( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686
}

void hal_outportb( uint16_t port, uint8_t data )
{
#ifdef CPU_ARCH_I686
	i686_outportb( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686
}

void hal_outportw( uint16_t port, uint16_t data )
{
#ifdef CPU_ARCH_I686
	i686_outportw( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686
}

void hal_outportl( uint16_t port, uint32_t data )
{
#ifdef CPU_ARCH_I686
	i686_outportl( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686
}

void hal_panic()
{
#ifdef CPU_ARCH_I686
	i686_panic();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686
}
