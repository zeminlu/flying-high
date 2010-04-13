OUTPUT_FORMAT("binary")
ENTRY (_loader)

SECTIONS{
    .text 0xFF800000 : {
	   *(.text)
    }
    .data : {
	   *(.data)
    }
    .bss :
    {
	   *(.bss)
    }
}
