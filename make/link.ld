ENTRY (start)
SECTIONS
{
	.text 0x100000 :
	{
		*(.boot)
		*(.text)
		. = ALIGN(4096);
	}
	. = ALIGN (CONSTANT (COMMONPAGESIZE));
	.data :
	{
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	.bss :
	{
		*(.bss)
		. = ALIGN(4096);
	}
	__end = .;
}
