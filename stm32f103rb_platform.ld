MEMORY
{
	FLASH	(rx)	: ORIGIN = 0x08000000, LENGTH = 128K
	RAM		(rwx)	: ORIGIN = 0x20000000, LENGTH =  20K
}

/* Symbol indicates the top of the stack.
 * Symbol used in the ISR vector table to set the initial stack pointer (SP). */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
	/*
	 * .text section:
	 * - Contains the executable code and read-only data
	 * - Placed in FLASH memory
	 */
	.text :
	{
		KEEP(*(.isr_vector));

		*(.text*)	/* section contains executable code */
		*(.rodata*)	/* section contains read-only data */

		/* Sections required for C++ compatibility with libraries */
		*(.init)	/* section contains initialization code */
		*(.fini)	/* section contains termination code */
		*(.eh_frame)/* section contains metadata for stack unwinding */
		*(.ARM.exidx)/* section contains unwinding table for ARM platform */

		/* Ensure alignment to 4 bytes for performance and correctness */
		. = ALIGN(4);

		/* __etext symbol marks end of text section */
		__etext = .;
	} > FLASH



	/* 
     * .data section:
     * - Contains initialized global and static variables
     * - Initial values of .data are stored in FLASH and copied to RAM
	 */
	.data : AT (__etext)
	{
		/* Symbol _sidata holds the start address of .data section in FLASH.
		 * Section .data contains initialization values and is stored in FLASH
		 * to prserve values after reset.
		 * LOADADDR(.data) retrievs the start address of the .data section. */
		_sidata = LOADADDR(.data); /* Address of .data section in FLASH */
		_sdata = .; /* start of .data section */
		*(.data*)
		_edata = .; /* end of .data section */
	} > RAM

	/* 
     * .bss section:
     * - Contains uninitialized global and static variables
     * - Also includes COMMON symbols which are uninitialized
     * - Placed in RAM memory
     * - Defines the _end symbol to mark the end of the BSS section
     */
	.bss :
	{
		_sbss = .; /* Start of .bss - required by startup script */
		__bss_start__ = _sbss; /* Start of .bss - required by newlib */

		*(.bss*)

		/* 
         * Include all COMMON symbols, which are uninitialized 
         * global variables shared across multiple object files.
         */
		*(COMMON)

		_ebss = .; /* End of .bss - required by startup script */
		__bss_end__ = _ebss; /* End of .bss - required by newlib */
		
		/*
         * Define the symbol _end at the current location.
         * This symbol marks the end of the BSS section and can be used
         * to determine where the heap starts.
		 * Required by newlib.
         */
        _end = .;
	} > RAM
}
