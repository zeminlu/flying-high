#include "kasm.h"
#include "kc.h"
#include "system.h"
#include "int_handlers.h"
#include "config.h"
#include "stdio.h"
#include "string.h"
#include "video_driver.h"
#include "mouse_driver.h"

DESCR_INT idt[256];			/* IDT de 256 entradas*/
IDTR idtr;					/* IDTR */

void kernel_main ( void  ) 
{
	
	char *msg;

	/* Loading IDT */
	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_handler, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_handler, ACS_INT, 0);
	setup_IDT_entry (&idt[0x74], 0x08, (dword)&_int_74_handler, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_handler, ACS_INT, 0);

	/* Loading IDTR */
	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	_lidt (&idtr);	


	clearScreen();

	msg = "\n\tWelcome to Ark OS 1.0\n\n";
	puts(msg);
	/* Enabling Interrupts */
	puts("\n\t\tEnabling Interrupts..............................................");
	_Cli();

		_mascaraPIC1(0xF8);
		_mascaraPIC2(0xEF);
        
	_Sti();
	puts("Done.\n");

	/* Initializing Driver */
	puts("\t\tInitializing Driver..............................................");
	initVideo(CURSOR_START_VISIBLE, POINTER_START_VISIBLE);
	initMouse();
	puts("Done.\n");
	
	puts("\n\tReady.\n\n");
	
	/* Main loop */
	launchApp(SHELL);
	while (1)
		runApp();
}
