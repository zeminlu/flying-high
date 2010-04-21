/*
 *	exceptions.c
 *
 * 		The implementation of all exception handlers.
 *
 *		Intel's 80386 Exceptions Table
 *		------------------------------
 *
 *		0            Divide error
 *		1            Debug exceptions
 *		2            Nonmaskable interrupt
 *		3            Breakpoint (one-byte INT 3 instruction)
 *		4            Overflow (INTO instruction)
 *		5            Bounds check (BOUND instruction)
 *		6            Invalid opcode
 *		7            Coprocessor not available
 *		8            Double fault
 *		9            (reserved)
 *		10           Invalid TSS
 *		11           Segment not present
 *		12           Stack exception
 *		13           General protection
 *		14           Page fault
 *		15           (reserved)
 *		16           Coprocessor error
 *		17-31        (reserved)
 *		
 *		Author: Luciano Zemin
 */
#include "../inc/exceptions.h"

extern DESCR_INT idt[];	

void _divide_e_hand();
void _debug_e_hand();
void _nmi_e_hand();
void _break_point_e_hand();
void _overflow_e_hand();
void _bounds_check_e_hand();
void _invalid_opcode_e_hand();
void _copro_unavailable_e_hand();
void _double_fault_e_hand();
void _invalid_tss_e_hand();
void _segment_not_present_e_hand();
void _stack_e_hand();
void _general_protection_e_hand();
void _page_fault_e_hand();
void _coprocessor_e_hand();

/*	
 *	Stack Order
 *
 *	.		0x00000000		
 *	.
 *	.
 *	
 *	ret
 *	edi
 *	esi
 *	ebp
 *	esp
 *	ebx
 *	edx
 *	ecx
 *	eax
 *	error
 *	ip
 *	cs	
 *	eflags
 *	
 *	.
 *	.
 *	.		0xFFFFFFFF
 *
 */
typedef struct registers {
	unsigned edi;
	unsigned esi;
	unsigned ebp;
	unsigned esp;
	unsigned ebx;
	unsigned edx;
	unsigned ecx;
	unsigned eax;
	unsigned error;
	unsigned eip;
	unsigned cs;
	unsigned eflags;
} registers_t;

/*
 *	Prints the KERNEL PANIC message and
 *	the message given.
 */
static void panic(char *msg, registers_t regs) {
	puts((const char *)"--------------------------------------------------------------------------------");
	puts((const char *)" KERNEL PANIC (");
	puts((const char *)msg);
	puts((const char *)") \n");

	puts((const char *)" Failing Instruction at: ");
	putx(regs.cs);
	puts((const char *)":");
	putx(regs.eip);
	puts((const char *)"\n\n");

	puts((const char *)" Registers: \n");
	puts((const char *)"     eax: ");
	putx(regs.eax);	
	puts((const char *)"\n");
	puts((const char *)"     ebx: ");
	putx(regs.ebx);
	puts((const char *)"\n");
	puts((const char *)"     ecx: ");
	putx(regs.ecx);
	puts((const char *)"\n");
	puts((const char *)"     edx: ");
	putx(regs.edx);
	puts((const char *)"\n");
	puts((const char *)"     esp: ");
	putx(regs.esp);
	puts((const char *)"\n");
	puts((const char *)"     ebp: ");
	putx(regs.ebp);
	puts((const char *)"\n");
	puts((const char *)"     esi: ");
	putx(regs.esi);
	puts((const char *)"\n");
	puts((const char *)"     edi: ");
	putx(regs.edi);
	puts((const char *)"\n");
	puts((const char *)"\n");
	return;
}

void handleException(registers_t regs) {
	/*
	 *	TODO check wich one was the failling
	 *	process and informe it, kill it and return
	 *	cp control to the scheduler
	 *	Probably this will be put in a separeted 
	 *	function.
	 */
	puts((const char *)"--------------------------------------------------------------------------------");
	
/*	sysKill(sysGetpid(), SIGKILL);
	signalTty(getFocusTTY());*/
while(1);	
	return;
}

void divideExceptionHandler(registers_t regs) {
	panic("Divide Error", regs);
	handleException(regs);
	return;
}

void debugExceptionHandler(registers_t regs) {
	panic("Debug Error", regs);
	handleException(regs);
	return;
}

void NMIExceptionHandler(registers_t regs) {
	panic("Nonmaskable Interrupt Error", regs);
	handleException(regs);
	return;
}

void breakPointExceptionHandler(registers_t regs) {
	panic("Break Point", regs);
	handleException(regs);
	return;
}

void overflowExceptionHandler(registers_t regs) {
	panic("Overflow Error", regs);
	handleException(regs);
	return;
}

void boundsCheckExceptionHandler(registers_t regs) {
	panic("Bounds Check Error", regs);
	handleException(regs);
	return;
}

void invalidOpcodeExceptionHandler(registers_t regs) {
	panic("Invalid Opcode", regs);
	handleException(regs);
	return;
}

void coprocessorUnavailableExceptionHandler(registers_t regs) {
	panic("Coprocessor Unavaible", regs);
	handleException(regs);
	return;
}

void doubleFaultExceptionHandler(registers_t regs) {
	panic("Double Fault", regs);
	handleException(regs);
	return;
}

void invalidTSSExceptionHandler(registers_t regs) {
	panic("Invalid TSS", regs);
	handleException(regs);
	return;
}

void segmentNotPresentExceptionHandler(registers_t regs) {
	panic("Segment not present", regs);
	handleException(regs);
	return;
}

void stackExceptionHandler(registers_t regs) {
	panic("Stack Error", regs);
	handleException(regs);
	return;
}

void generalProtectionExceptionHandler(registers_t regs) {
	panic("General Protection Error", regs);
	handleException(regs);
	return;
}

void pageFaultExceptionHandler(registers_t regs) {
	unsigned address;
	panic("Page Fault", regs);

	asm volatile("mov %%cr2, %0": "=r"(address));
	puts((const char *)" Trying to access address ");
	putx(address);
	puts((const char *)"\n\n");
	puts((const char *)" Fault Info: \n");
	if ( (regs.error & 0x1) )
		puts((const char *)"     1 - The fault was caused by a page-level protection violation.\n");
	else
		puts((const char *)"     1 - The fault was caused by a non-present page.\n");

	if ( (regs.error & 0x2) )
		puts((const char *)"     2 - The access causing the fault was write.\n");
	else
		puts((const char *)"     2 - The access causing the fault was read.\n");

	if ( (regs.error & 0x4) )
		puts((const char *)"     3 - The access causing the fault oriented when the processor was executing in user mode.\n");
	else
		puts((const char *)"     3 - The access causing the fault oriented when the processor was executing in kernel mode.\n");

	if ( (regs.error & 0x8) )
		puts((const char *)"     4 - The fault was caused by reserved bits set to 1 in page directory.\n");
	else
		puts((const char *)"     4 - The fault was not caused by reserved bits violation.\n");

	handleException(regs);
	return;
}

void coprocessorExceptionHandler(registers_t regs) {
	panic("Coprocessor Error", regs);
	return;
}

/*
 *	TODO Check wich one of the interrupt requires
 *	that a singal to the pic must be send.
 *
 */

void loadExceptionHandlers() {
	setup_IDT_entry (&idt[0x00], 0x08, (dword)&_divide_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x01], 0x08, (dword)&_debug_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x02], 0x08, (dword)&_nmi_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x03], 0x08, (dword)&_break_point_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x04], 0x08, (dword)&_overflow_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x05], 0x08, (dword)&_bounds_check_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x06], 0x08, (dword)&_invalid_opcode_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x07], 0x08, (dword)&_copro_unavailable_e_hand, ACS_INT, 0);
	/*//setup_IDT_entry (&idt[0x08], 0x08, (dword)&_double_fault_e_hand, ACS_INT, 0); 
	//setup_IDT_entry (&idt[0xA0], 0x08, (dword)&_invalid_tss_e_hand, ACS_INT, 0); */
	setup_IDT_entry (&idt[0x0B], 0x08, (dword)&_segment_not_present_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0C], 0x08, (dword)&_stack_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0D], 0x08, (dword)&_general_protection_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0E], 0x08, (dword)&_page_fault_e_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x10], 0x08, (dword)&_coprocessor_e_hand, ACS_INT, 0);
}
