#ifndef ARCH_X86_GDT_H
#define ARCH_X86_GDT_H

// Kernel code and data segments in GDT
// NOTE: If you change it you also need to update same values in arch/x86/entry.asm
#define GDT_KERNEL_CODE_SEGMENT 		0x08
#define GDT_KERNEL_DATA_SEGMENT 		0x10

// Initialize global descriptor table
void 	x86_gdt_init();

#endif // !ARCH_X86_GDT_H
