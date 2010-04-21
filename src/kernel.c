#include "kernel.h"

DESCR_INT idt[256];			/* IDT de 256 entradas*/
IDTR idtr;					/* IDTR */

void kernel_main ( void  ) 
{
	char *msg;/*, *aux = "Anduvo\n";
	frame_t *frame;*/
	char *buffer;

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

	clearScreen();

	msg = "\n\tWelcome, you're Flying-High at 0.2 meters!!\n\n";
	puts(msg);
	
	initPaging();
	
	/* Enabling Interrupts */
	puts("\n\t\tEnabling Interrupts..............................................");
	_Cli();

		_mascaraPIC1(0xF8);
		_mascaraPIC2(0xFF);
        
	_Sti();
	puts("Done.\n");

	/* Initializing Driver */
	puts("\t\tInitializing Driver..............................................");
	initVideo(CURSOR_START_VISIBLE, DISABLED);
	/*initMouse();*/
	puts("Done.\n");
	
	puts("\n\tReady.\n\n");
	
	buffer = kMalloc(sizeof(char) * 6);
	buffer[0] = 'H';
	buffer[1] = 'o';
	buffer[2] = 'l';
	buffer[3] = 'a';
	buffer[4] = '\n';
	buffer[5] = '\0';
	
	puts(buffer);
	puts("Hizo kMalloc\n");
	flushKeyboardBuffer(); /* flush kb buffer into stdin */
	flushStdScreen();	/* flush stdout into video buffer and refresh screen */
	/* Main loop */
	launchApp(SHELL);
	while (1)
		runApp();
}
