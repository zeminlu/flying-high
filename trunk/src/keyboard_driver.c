/*
 * 	file: keyboard_driver.c
 *
 * 	\brief: This file contains the scancode table for all of the letter and
 * 			their combination.
 *			-The incoming scancode of the keyboard select the row
 *			-The modifier status select the column
 *			-The intersection of two is the scan/ASCII code to put into the PC buffer ahead	
 */

#include "io.h"
#include "keyboard_driver.h"
#include "config.h"

/* ----------------------------------------------------------------------
 * Software keycodes
 * ---------------------------------------------------------------------- */

/*
 * 	\brief Each keyboard event generates a 16 bit code.
 * 			- The low 10 bits indicate which key was used.
 * 			- If bit 8 (KEY_SPECIAL_FLAG) is 0, then the low 8 bits contain
 *   		  the ASCII code.
 * 			- The flags indicate the shift/alt/control state,
 *   		  and whether the event was a make or release.
 */

static char flag = 0;
static char flagCaps = 0;
static int amountOfRepetition = KEYBOARD_REPETITION;
static int keyboardActivity = 0;

#define KEY_RSHIFT_P	0x2A
#define KEY_LSHIFT_P	0x36
#define KEY_RSHIFT_R	(KEY_RSHIFT_P + KB_KEY_RELEASE)
#define KEY_LSHIFT_R	(KEY_LSHIFT_P + KB_KEY_RELEASE)
#define KEY_ALT_P		0x38
#define KEY_ALT_R		(KEY_ALT_P + KB_KEY_RELEASE)
#define KEY_CTRL_P		0x1D
#define	KEY_CTRL_R		(KEY_CTRL_P+KB_KEY_RELEASE)
#define KEY_CAPS		0x3A

#define KB_KEY_RELEASE	0x80
#define KB_OUTPUT_FULL	0x01

#define DATA_PORT		0x60
#define CONTROL_PORT	0x64

/*
 * Flags
 */
#define FLAG_SHIFT		(0x01)
#define FLAG_CTRL		(0x02)
#define FLAG_ALT		(0x04)
#define FLAG_CAPS		(0x08)

#define KEY_SPECIAL_FLAG 0x00
#define KEY_KEYPAD_FLAG  0x00
#define KEY_SHIFT_FLAG   0x1000
#define KEY_ALT_FLAG     0x2000
#define KEY_CTRL_FLAG    0x4000
#define KEY_RELEASE_FLAG 0x8000

/*
 * Special key codes
 */

#define _SPECIAL(num) (KEY_SPECIAL_FLAG & (num))
#define KEY_UNKNOWN _SPECIAL(0) 
#define KEY_F1      KB_KEY_RELEASE & 0x01
#define KEY_F2      KB_KEY_RELEASE & 0x02 
#define KEY_F3      KB_KEY_RELEASE & 0x03 
#define KEY_F4      KB_KEY_RELEASE & 0x04 
#define KEY_F5      KB_KEY_RELEASE & 0x05 
#define KEY_F6      KB_KEY_RELEASE & 0x06
#define KEY_F7      KB_KEY_RELEASE & 0x07
#define KEY_F8      KB_KEY_RELEASE & 0x08
#define KEY_F9      _SPECIAL(9) 
#define KEY_F10     _SPECIAL(10) 
#define KEY_F11     _SPECIAL(11) 
#define KEY_F12     _SPECIAL(12) 
#define KEY_LCTRL   _SPECIAL(13) 
#define KEY_RCTRL   _SPECIAL(14) 
#define KEY_LSHIFT  _SPECIAL(15) 
#define KEY_RSHIFT  _SPECIAL(16) 
#define KEY_LALT    _SPECIAL(17) 
#define KEY_RALT    _SPECIAL(18) 
#define KEY_PRINTSCRN _SPECIAL(19) 
#define KEY_CAPSLOCK _SPECIAL(20) 
#define KEY_NUMLOCK _SPECIAL(21) 
#define KEY_SCRLOCK _SPECIAL(22) 
#define KEY_SYSREQ  _SPECIAL(23) 

/*
 * Keypad keys
 */
#define KEYPAD_START 128
#define _KEYPAD(num) (KEY_KEYPAD_FLAG & KEY_SPECIAL_FLAG & (num+KEYPAD_START))
#define KEY_KPHOME  _KEYPAD(0)
#define KEY_KPUP    _KEYPAD(1)
#define KEY_KPPGUP  _KEYPAD(2)
#define KEY_KPMINUS _KEYPAD(3)
#define KEY_KPLEFT  _KEYPAD(4)
#define KEY_KPCENTER _KEYPAD(5)
#define KEY_KPRIGHT _KEYPAD(6)
#define KEY_KPPLUS  _KEYPAD(7)
#define KEY_KPEND   _KEYPAD(8)
#define KEY_KPDOWN  _KEYPAD(9)
#define KEY_KPPGDN  _KEYPAD(10)
#define KEY_KPINSERT _KEYPAD(11)
#define KEY_KPDEL   _KEYPAD(12)

#define ASCII_ESC	0x00
#define ASCII_BS	'\b'
#define ASCII_ENTER '\n'
/*
 * Translate from scan code to key code, when shift is not pressed.
 */

static const Keycode scanTableNoShift[] = {
	KEY_UNKNOWN, ASCII_ESC, '1', '2',   /* 0x00 - 0x03 */
	'3', '4', '5', '6',                 /* 0x04 - 0x07 */
	'7', '8', '9', '0',                 /* 0x08 - 0x0B */
	'-', '=', ASCII_BS, '\t',           /* 0x0C - 0x0F */
	'q', 'w', 'e', 'r',                 /* 0x10 - 0x13 */
	't', 'y', 'u', 'i',                 /* 0x14 - 0x17 */
	'o', 'p', '[', ']',                 /* 0x18 - 0x1B */
	ASCII_ENTER, KEY_LCTRL, 'a', 's',   /* 0x1C - 0x1F */
	'd', 'f', 'g', 'h',                 /* 0x20 - 0x23 */
	'j', 'k', 'l', ';',                 /* 0x24 - 0x27 */
	'\'', '`', KEY_LSHIFT, '\\',        /* 0x28 - 0x2B */
	'z', 'x', 'c', 'v',                 /* 0x2C - 0x2F */
	'b', 'n', 'm', ',',                 /* 0x30 - 0x33 */
	'.', '/', KEY_RSHIFT, KEY_PRINTSCRN, /* 0x34 - 0x37 */
	KEY_LALT, ' ', KEY_CAPSLOCK, KEY_F1, /* 0x38 - 0x3B */
	KEY_F2, KEY_F3, KEY_F4, KEY_F5,     /* 0x3C - 0x3F */
	KEY_F6, KEY_F7, KEY_F8, KEY_F9,     /* 0x40 - 0x43 */
	KEY_F10, KEY_NUMLOCK, KEY_SCRLOCK, KEY_KPHOME,  /* 0x44 - 0x47 */
	KEY_KPUP, KEY_KPPGUP, KEY_KPMINUS, KEY_KPLEFT,  /* 0x48 - 0x4B */
	KEY_KPCENTER, KEY_KPRIGHT, KEY_KPPLUS, KEY_KPEND,  /* 0x4C - 0x4F */
	KEY_KPDOWN, KEY_KPPGDN, KEY_KPINSERT, KEY_KPDEL,  /* 0x50 - 0x53 */
	KEY_SYSREQ, KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN,  /* 0x54 - 0x57 */
};

#define SCAN_TABLE_SIZE (sizeof(s_scanTableNoShift) / sizeof(Keycode))

/*
 * \brief 	Translate from scan code to key code, when shift *is* pressed.
 * 			Keep this in sync with the unshifted table above!
 * 			They must be the same size.
 */
static const Keycode scanTableWithShift[] = {
	KEY_UNKNOWN, ASCII_ESC, '!', '@',   /* 0x00 - 0x03 */
	'#', '$', '%', '^',                 /* 0x04 - 0x07 */
	'&', '*', '(', ')',                 /* 0x08 - 0x0B */
	'_', '=', ASCII_BS, '\t',           /* 0x0C - 0x0F */
	'Q', 'W', 'E', 'R',                 /* 0x10 - 0x13 */
	'T', 'Y', 'U', 'I',                 /* 0x14 - 0x17 */
	'O', 'P', '{', '}',                 /* 0x18 - 0x1B */
	ASCII_ENTER, KEY_LCTRL, 'A', 'S',   /* 0x1C - 0x1F */
	'D', 'F', 'G', 'H',                 /* 0x20 - 0x23 */
	'J', 'K', 'L', ':',                 /* 0x24 - 0x27 */
	'"', '~', KEY_LSHIFT, '|',          /* 0x28 - 0x2B */
	'Z', 'X', 'C', 'V',                 /* 0x2C - 0x2F */
	'B', 'N', 'M', '<',                 /* 0x30 - 0x33 */
	'>', '?', KEY_RSHIFT, KEY_PRINTSCRN, /* 0x34 - 0x37 */
	KEY_LALT, ' ', KEY_CAPSLOCK, KEY_F1, /* 0x38 - 0x3B */
	KEY_F2, KEY_F3, KEY_F4, KEY_F5,     /* 0x3C - 0x3F */
	KEY_F6, KEY_F7, KEY_F8, KEY_F9,     /* 0x40 - 0x43 */
	KEY_F10, KEY_NUMLOCK, KEY_SCRLOCK, KEY_KPHOME,  /* 0x44 - 0x47 */
	KEY_KPUP, KEY_KPPGUP, KEY_KPMINUS, KEY_KPLEFT,  /* 0x48 - 0x4B */
	KEY_KPCENTER, KEY_KPRIGHT, KEY_KPPLUS, KEY_KPEND,  /* 0x4C - 0x4F */
	KEY_KPDOWN, KEY_KPPGDN, KEY_KPINSERT, KEY_KPDEL,  /* 0x50 - 0x53 */
	KEY_SYSREQ, KEY_UNKNOWN, KEY_UNKNOWN, KEY_UNKNOWN,  /* 0x54 - 0x57 */
};


Keycode keyboardBuffer[KEYBOARD_BUFFER_SIZE];

/* ----------------------------------------------------------------------
 * Private data and functions
 * ---------------------------------------------------------------------- */



void keyboard_driver(){

	Keycode scan = 0, auxScan, oldScanCode=0;
	unsigned char status;
	int countEnq = 0;
	
	status = inportb(CONTROL_PORT);
	if( (KB_OUTPUT_FULL & status) != 0 )
		scan = inportb(DATA_PORT);			/* The keyboard is avaliable to read */
	SetKeyState(scan);
	if( !(scan & KB_KEY_RELEASE)  )
	{
		if( flag & FLAG_SHIFT  )
		{
			if( flagCaps & FLAG_CAPS )
			{
				auxScan = scanTableNoShift[scan];
				if( auxScan >= 'a' && auxScan <= 'z' )
				{
					charEnque(auxScan);
				}
				else
				{	
					auxScan = scanTableWithShift[scan];
					if( auxScan != 0 )
						charEnque(auxScan);
				}
			}else
			{
				auxScan = scanTableWithShift[scan];
					if( auxScan != 0 )
						charEnque(auxScan);
			}
		}
		else if( flag & FLAG_CAPS && (auxScan = scanTableNoShift[scan]) >= 'a' && auxScan <= 'z' )
		{
			auxScan -= 0x20;
			if( auxScan != 0 )
				charEnque(auxScan);
		}
		else
		{
			auxScan = scanTableNoShift[scan];
			if( oldScanCode == scan )
				while( countEnq++ <= amountOfRepetition )
					charEnque(auxScan);
			else if( auxScan )
				charEnque(auxScan);
		}

	}
	oldScanCode = scan;
	keyboardActivity = 1;
}

void SetKeyState( Keycode scanCode )
{
	switch (scanCode) {
		case KEY_RSHIFT_P: case KEY_LSHIFT_P:
			flag |= FLAG_SHIFT;
			break;
		case KEY_RSHIFT_R: case KEY_LSHIFT_R:
			flag &= ~FLAG_SHIFT;
			break;
		case KEY_CAPS:
			flag = flag ^FLAG_CAPS;
			flagCaps = flagCaps ^FLAG_CAPS;
			break;
		case KEY_CTRL_P:
			flag |= FLAG_CTRL;
			break;
		case KEY_CTRL_R:
			flag &= ~FLAG_CTRL;
			break;
		}
}


int shiftIsPressed( void )
{
	return flag == FLAG_SHIFT;
}

int capsIsPressed( void )
{
	return flag == FLAG_CAPS;
}

int ctrlIsPressed ( void )
{
	return flag == FLAG_CTRL;
}

int getRepetition()
{
	return amountOfRepetition;
}

void setRepetition( int newRep )
{
	amountOfRepetition = newRep;
}

int checkKeyboardActivity( void )
{
	if( keyboardActivity )
	{	
		keyboardActivity = 0;
		return 1;
	}
	return 0;
}

void setKeyboardActivity(void)
{ 
	keyboardActivity = 0;
}
