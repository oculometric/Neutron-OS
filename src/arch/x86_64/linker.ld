ENTRY (start)

SECTIONS {
	kernel_start = . ;
	.boot 0x100000 :
	{
		*(.multiboot_header)
	}
	.early (kernel_start + 0x80) :
	{
		*(.early)
	}
	.text (kernel_start + 0x2000) :
	{
		*(.text)
	}
	.data :
	{
		*(.data)
	}
	.bss :
	{
		*(.bss)
	}
	kernel_end = . ;
	kernel_size = kernel_end - kernel_start;
}
