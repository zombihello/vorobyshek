#include "system/hal.h"

// Build kernel for x32 or x86_64 architecture
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	#include "arch/i686/io.h"
	#include "arch/i686/gdt.h"
	#include "arch/i686/idt.h"
	#include "arch/i686/isr.h"
	#include "arch/i686/cpu.h"
	#include "arch/i686/irq.h"
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64

void hal_init()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_gdt_init();
	i686_idt_init();
	i686_isr_init();
	i686_irq_init();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

uint8_t hal_inportb( uint16_t port )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	return i686_inportb( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

uint16_t hal_inportw( uint16_t port )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	return i686_inportw( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

uint32_t hal_inportl( uint16_t port )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	return i686_inportl( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_outportb( uint16_t port, uint8_t data )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_outportb( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_outportw( uint16_t port, uint16_t data )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_outportw( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_outportl( uint16_t port, uint32_t data )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_outportl( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_iowait()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_iowait();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_panic()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_panic();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_enable_interrupts()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_enable_interrupts();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_disable_interrupts()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	i686_disable_interrupts();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}
