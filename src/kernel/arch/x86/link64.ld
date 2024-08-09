/* We want the symbol 'start' to be our entry point */
ENTRY( start )

KERNEL_PHYSICAL_MEM 	= 0x10000;
KERNEL_OFFSET_MEM 		= 0xFFFFFF8000000000;

SECTIONS
{
	/* Place our bootstrap in lower half memory (physical memory) */
	. = KERNEL_PHYSICAL_MEM;

    .bootstrap :
    {
		*(.multiboot)
        *(.init.text)
		*(.init.data)
    }

	/* Another parts we place in higher half memory (virtual memory) */
    . += KERNEL_OFFSET_MEM;

	/* Code section */
    .text ALIGN( 4K ) : AT( ADDR( .text ) - KERNEL_OFFSET_MEM )
    {
        *(.text)
    }

	/* Read-only data section */
	.rodata ALIGN( 4K ) : AT( ADDR( .rodata ) - KERNEL_OFFSET_MEM )
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
   .data ALIGN( 4K ) : AT( ADDR( .data ) - KERNEL_OFFSET_MEM )
   {
        *(.data)
   }

   /* Read-write data (uninitialized) and stack */
   .bss ALIGN( 4K ) : AT( ADDR( .bss ) - KERNEL_OFFSET_MEM )
   {
       *(COMMON)
       *(.bss)
   }
}
