void x86_panic()
{
	__asm__ __volatile__(
			"cli\n\t"
			"hlt\n\t"
		   );
}

void x86_enable_interrupts()
{
	__asm__ __volatile__(
			"sti\n\t"
			);
}

void x86_disable_interrupts()
{
	__asm__ __volatile__(
			"cli\n\t"
			);
}
