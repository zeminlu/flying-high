#include "kernel.h"

DESCR_INT idt[256];			/* IDT de 256 entradas*/
IDTR idtr;					/* IDTR */

void kernel_main ( void  ) 
{
	/*int status;*/
	char *msg;
	
	_Cli();
		_mascaraPIC1(0xFF);
		_mascaraPIC2(0xFF);
	_Sti();
	
	/* Loading IDT */
	/*setup_IDT_entry (&idt[0x74], 0x08, (dword)&_int_74_handler, ACS_INT, 0);*/
	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_handler, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_handler, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_handler, ACS_INT, 0);
	loadExceptionHandlers();
	

	/* Loading IDTR */
	idtr.base = 0;
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	_lidt (&idtr);	

	initPaging();
				
	clearScreen();
	
	initializeTTY();

	initializeShMems();
	
	initializeSemaphores();

	msg = "\n\tWelcome, you're Flying-High at 0.6 meters!!\n\n";
/*	putsInStdTTY(msg);
	
	// Initializing Driver
	putsInStdTTY("\t\tInitializing Drivers..............................................");
	
	
	//initMouse();
	putsInStdTTY("Done.\n");
	
	// Enabling Interrupts
	putsInStdTTY("\n\t\tEnabling Interrupts..............................................");
	putsInStdTTY("Done.\n");
	
	putsInStdTTY("\n\tReady.\n\n");
*/
	initVideo(CURSOR_START_VISIBLE, DISABLED);

	initMultitasker(init);
	
	_Cli();
		_mascaraPIC1(0xFC);
		_mascaraPIC2(0xFF);
	_Sti();
	
	/*waitpid(0, &status);
	
	
	*/
	/* Main loop */
	
	/*launchApp(SHELL);*/
	while (1)

		asm volatile("hlt");
}
