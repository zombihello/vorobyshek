#ifndef ARCH_I686_GDT_H
#define ARCH_I686_GDT_H

// Kernel code and data segments in GDT
// NOTE: If you change it you also need to update same values in arch/i686/entry.asm
#define GDT_KERNEL_CODE_SEGMENT 		0x08
#define GDT_KERNEL_DATA_SEGMENT 		0x10

// Initialize global descriptor table
void 	i686_gdt_init();

#endif // !ARCH_I686_GDT_H
