#ifndef ARCH_X86_CPU_H
#define ARCH_X86_CPU_H

// Halt the CPU
void 	x86_panic();

// Enable and disable interrupts
void 	x86_enable_interrupts();
void 	x86_disable_interrupts();

#endif // !ARCH_X86_CPU_H
