#include "system/hal.h"

// Build kernel for x32 or x86_64 architecture
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	#include "arch/x86/io.h"
	#include "arch/x86/gdt.h"
	#include "arch/x86/idt.h"
	#include "arch/x86/isr.h"
	#include "arch/x86/cpu.h"
	#include "arch/x86/irq.h"
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64

void hal_init()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_gdt_init();
	x86_idt_init();
	x86_isr_init();
	x86_irq_init();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

uint8_t hal_inportb( uint16_t port )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	return x86_inportb( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

uint16_t hal_inportw( uint16_t port )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	return x86_inportw( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

uint32_t hal_inportl( uint16_t port )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	return x86_inportl( port );
#else
	#error Unknown CPU architecture
	return 0;
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_outportb( uint16_t port, uint8_t data )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_outportb( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_outportw( uint16_t port, uint16_t data )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_outportw( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_outportl( uint16_t port, uint32_t data )
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_outportl( port, data );
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_iowait()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_iowait();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_panic()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_panic();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_enable_interrupts()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_enable_interrupts();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}

void hal_disable_interrupts()
{
#if defined( CPU_ARCH_I686 ) || defined( CPU_ARCH_AMD64 )
	x86_disable_interrupts();
#else
	#error Unknown CPU architecture
#endif // CPU_ARCH_I686 || CPU_ARCH_AMD64
}
