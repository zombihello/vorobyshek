void i686_panic()
{
	__asm__ __volatile__(
			"cli\n\t"
			"hlt\n\t"
		   );
}

void i686_enable_interrupts()
{
	__asm__ __volatile__(
			"sti\n\t"
			);
}

void i686_disable_interrupts()
{
	__asm__ __volatile__(
			"cli\n\t"
			);
}
