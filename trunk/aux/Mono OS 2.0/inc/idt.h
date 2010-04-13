/*
 *	idt.h
 *
 *		Defines the function that loads and interrupt or exception
 *		handler into an IDT.
 */
#ifndef IDT_H_
#define IDT_H_

#include "defs.h"

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access, byte cero);
#endif
