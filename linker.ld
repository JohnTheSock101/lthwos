ENTRY(_start)

SECTIONS
{

	/* offset of 2M to be safe; earlier memory can overwritten */

	. = 2M;

	/* multiboot header; needed early in the file so the bootloader notices */

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}


	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

}
