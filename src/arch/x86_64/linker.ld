ENTRY (start)

SECTIONS {
	kernel_start = 0x80100000;
	.boot 0x100000 :
	{
		*(.multiboot_header)
	}
	.early 0x80100080 :
	{
		*(.early)
	}
	.text 0x80102000 : AT(0x80102000)
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
