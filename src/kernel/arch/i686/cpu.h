#ifndef ARCH_I686_CPU_H
#define ARCH_I686_CPU_H

// Halt the CPU
void 	i686_panic();

// Enable and disable interrupts
void 	i686_enable_interrupts();
void 	i686_disable_interrupts();

#endif // !ARCH_I686_CPU_H
