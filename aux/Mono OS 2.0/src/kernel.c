/*
 * File: kernel.c
 * Description: Kernel
 */

/*
 * Includes
 */
#include "kasm.h"
#include "defs.h"
#include "intHandlers.h"
#include "videoDriver.h"
#include "mouseInit.h"
#include "shell.h"
#include "screenSaver.h"
#include "memmanager.h"
#include "idt.h"
#include "exceptions.h"
#include "string.h"
#include "sysprocess.h"
#include "debug.h"
#include "sysfilesystem.h"
#include "bin.h"

DESCR_INT idt[0x90];	
IDTR idtr;			
#define TIMER_TICKS 18
#define SEGS 20

int tickpos=640;
int tickCount=0;

/*
 * Functions
 */


/*
 * 	Function: void kmain()
 * 	Description: Runs monoOS
 */
void
kmain() 
{
	_Cli();
		_mascaraPIC1(0xFF);
		_mascaraPIC2(0xFF);
	_Sti();
	
	/* loads the IDT with the interruption rutines */
  	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x16], 0x08, (dword)&_int_16_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x20], 0x08, (dword)&_int_20_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x74], 0x08, (dword)&_int_74_hand, ACS_INT, 0);
	loadExceptionHandlers();
	
	/* loads the IDTR */
	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	
	
	/* Initializes The File System */
	initFileSystem( 50/*open files*/,50/*files*/, 20/*tags*/);
	
	/* Initializes The Screen */
	initializeScreen(18 ,NO_MOUSE_FORMAT);
	
	/* Initializes all the TTYs */
	initializeTTY();
	/* Initializes Paging Mode */
	if ( !initPageDirectory() ) {
		puts((unsigned char *)"Paging Mode Could not be Initialized.\n");
		while(1);
	}
	/* Initializes The Multi-tasker */
	if ( !initMultitasker(init) ) {
		puts((unsigned char *)"ERROR: Multitasker could not be initialized: ");
		puts((unsigned char *)getProcessErrorMsg());
		while(1);
	}

	/* Enables the pertinent interruptions */
	_Cli();
	//	mouseinit();
		_mascaraPIC1(0xf8);
		_mascaraPIC2(0xEF);
	_Sti();
	
//	shellStart();
	
	while(1){}
}

