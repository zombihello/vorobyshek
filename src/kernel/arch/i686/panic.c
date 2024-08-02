void i686_panic()
{
	__asm__(
			"cli\n\t"
			"hlt\n\t"
		   );
}
