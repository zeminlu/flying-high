/*
 * \file mouse_driver.c
 *
 *	\author: Guido Marucci Blas & Nicolas Purita & Luciano Zemin
 */

/*---------INCLUDE SECTION----------*/

#include "mouse_driver.h"

/*----------DEFINE SECTION----------*/

#define ENABLE_IRQ12 0x02
#define DISABLE_MOUSE_CLOCK 0xDF
#define READ 0
#define WRITE 1
#define OLD 0
#define NEW 1
#define TMP 2
#define LEFT_OLD 0
#define RIGHT_OLD 1
#define LEFT_NEW 2
#define RIGHT_NEW 3
#define UPPER_X 0
#define UPPER_Y 1
#define LOWER_X 2
#define LOWER_Y 3

/*----------------------------MOUSE INITIALIZATION----------------------------*/

static void mouseWaitTo (int mode)
{
	int graceperiod = 100000;

	if (mode == READ)
		while (((inportb(0x64) & 0x01) != 1) && graceperiod--);
	else
		while (((inportb(0x64) & 0x02) != 0) && graceperiod--);
	return;
}

static void mouseWrite (unsigned char byte)
{
	mouseWaitTo(WRITE);
	outportb(0x64, byte);
	return;
}

static unsigned char mouseRead (void)
{
	mouseWaitTo(READ);
	return inportb(0x60);
}

static void mouseSendCommand(unsigned char command)
{
	mouseWaitTo(WRITE);
	outportb(0x64, 0xD4);

	mouseWaitTo(WRITE);
	outportb(0x60, command);

	mouseRead();

	return;
}

void initMouse(void)
{
	unsigned char byte;
	
	mouseWrite(0xA8);
	mouseRead();
	mouseWrite(0x20);
	
	byte = mouseRead();
	byte = byte | ENABLE_IRQ12;
	byte = byte & DISABLE_MOUSE_CLOCK;
	
	mouseRead();

	mouseWrite(0x60);
	mouseWaitTo(WRITE);
	outportb(0x60, byte);
	mouseRead();
	
	mouseSendCommand(0xF6);
	mouseSendCommand(0xF4);
	
	return;
}

/*-------------------------------MOUSE DRIVER---------------------------------*/

static int left = 0, right = 0, col2 = POINTER_START_COL, row2 = POINTER_START_ROW, oldAmount = 0, counter = 0, rate = 0;

static unsigned char bytes[6];

static void checkPosition (int *curCol, int *curRow)
{
	*curCol = col2;
	*curRow = row2;
	return;
}
static void checkButtons (int *leftButton, int *rightButton, int *pressed)
{
	*leftButton = left;
	*rightButton = right;
	*pressed = oldAmount;
	return;
}

void mouseDriver(void)
{
	static int cycles = 0;
	int amount = 0;

	bytes[counter++] = mouseRead();
	
	if (counter == 3)
	{				
		counter = 0;
		if (((bytes[0] & 0x80) == 0x80) || ((bytes[0] & 0x40) == 0x40))
			return;
		
		amount = (left = ((bytes[0] & 0x01) == 0x01));
		amount += (right = ((bytes[0] & 0x02) == 0x02));
		amount += ((bytes[0] & 0x04) == 0x04);
	
		if (oldAmount == amount)
		{
			col2 += (((int)bytes[1]) - (256 * ((bytes[0] & 0x10) == 0x10)));
			col2 = (col2 >= SCREEN_WIDTH) ? SCREEN_WIDTH - 1: col2;
			col2 = (col2 < 0) ? 0 : col2;
				
			row2 -= (((int)bytes[2]) - (256 * ((bytes[0] & 0x20) == 0x20)));
			row2 = (row2 >= SCREEN_HEIGTH) ? SCREEN_HEIGTH - 1: row2;
			row2 = (row2 < 0) ? 0 : row2;
		}
		else
			oldAmount = amount;
	}
	if (cycles == rate)
	{
		refreshMouse();
		cycles = 0;
	}
	else
		++cycles;

	return;
}

/*-------------------------------MOUSE REFRESHING-----------------------------*/

static int buttons[4] = {0, 0, 0, 0}, col[3] = {0, 0, 0}, row[3] = {0, 0, 0}, points[4] = {0, 0, 0, 0}, amount[2] = {0, 0}, copying = 0, copyed = 0, painting = 0, state = 0, enabled = 1;

static char buffer[VIDEO_MEMORY_SIZE], buf1[SCREEN_SIZE + 21], buf2[SCREEN_SIZE + 21];

int checkMouseActivity(void)
{
	if (state)
	{
		state = 0;
		return 1;
	}
	return 0;
}

static void setCoordinates(void)
{
	points[UPPER_X] = col[OLD];
	points[UPPER_Y] = row[OLD];
	points[LOWER_X] = col[NEW];
	points[LOWER_Y] = row[NEW];

	return;
}

static void checkCoordinates(void)
{
	int tmp = 0;

	if (points[UPPER_X] > points[LOWER_X])
	{
		tmp = points[UPPER_X];
		points[UPPER_X] = points[LOWER_X];
		points[LOWER_X] = tmp;
	}
	
	if (points[UPPER_Y] > points[LOWER_Y])
	{		
		tmp = points[UPPER_Y];
		points[UPPER_Y] = points[LOWER_Y];
		points[LOWER_Y] = tmp;
	}

	return;
}

static void paste(char *area, int size, int width)
{
	int i, j;

	for (i = 0 ; i < size / width ; ++i)
	{
		for (j = 0 ; j < width * 2 ; j += 2)
		{
			buf1[(i * (width + 1)) + j / 2] = area[2 * (i * width) + j + 1];
			buf2[(i * (width + 1)) + j / 2] = area[2 * (i * width) + j];
		}
		buf1[(i * (width + 1)) + j / 2] = VIDEO_COLOR;
		buf2[(i * (width + 1)) + j / 2] = '\n';
	}

	write(STDIN, (void *)buf2, size + size / width - 1);
	write(IN_ATT, (void *)buf1, size + size / width - 1);

	return;
}

static void processButton(int press)
{
	switch (press)
	{
		case 1:
			if (painting)
			{
				checkCoordinates();
				paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
				updateColor();
			}
			copying = painting = 1;
			copyed = 0;
			checkPosition(&col[OLD], &row[OLD]);
			col[NEW] = col[OLD];
			row[NEW] = row[OLD];
			setCoordinates();
			checkCoordinates();
			paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
			break;
		case 2:
			if (copyed)
			{
				checkCoordinates();
				if (painting)
				{
					paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
					updateColor();
				}
				paste(buffer, (points[LOWER_X] - points[UPPER_X] + 1) * (points[LOWER_Y] - points[UPPER_Y] + 1), (points[LOWER_X] - points[UPPER_X]) + 1);
				painting = 0;
			}
			break;
		case -1:
			if (painting)
			{
				checkCoordinates();
				paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
				updateColor();
			}
			checkPosition(&col[NEW], &row[NEW]);
			setCoordinates();
			checkCoordinates();
			paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
			copyArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y], buffer);
			copying = 0;
			painting = copyed = 1;
			break;
		case -2:case 3:case -3:
			break;
		default:
			break;
	}
	return;
}

static void cleanIfNeeded(void)
{
	if (painting)
	{
		if (copying)
		{
			checkPosition(&col[NEW], &row[NEW]);	
			setCoordinates();
		}
		checkCoordinates();
		paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
		updateColor();
		copyed = copying = painting = 0;
	}
	return;
}

void setMouseState(int newState)
{
	enabled = newState;
	return;
}

void setMouseRate (int newRate)
{
	rate = newRate;
	return;
}

void refreshMouse (void)
{
	int press = 0, i;
	
	checkButtons(&buttons[LEFT_NEW], &buttons[RIGHT_NEW], &amount[NEW]);
	
	if (amount[NEW] != amount[OLD])
	{
		if (enabled)
		{
			if (amount[NEW] > 1 || amount[OLD] > 1)
				cleanIfNeeded();
			else
			{
				for (i = 0 ; i < 2  ; ++i) 
					if (buttons[i] != buttons[i + 2])
					{
						buttons[i] = buttons[i + 2];
						press = (buttons[i] ? (i + 1) : ((i + 1) * (-1)));
						break;
					}
				processButton(press);
			}
			amount[OLD] = amount[NEW];
		}
		state = 1;
	}
	else
	{
		checkPosition(&col[NEW], &row[NEW]);
		if (row[TMP] != row[NEW] || col[TMP] != col[NEW])
		{
			if (enabled)
			{
				setPointerPosition(row[NEW], col[NEW]);
				row[TMP] = row[NEW];
				col[TMP] = col[NEW];
				
				if (copying)
				{
					if (painting)
					{
						checkCoordinates();
						paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
						updateColor();
					}
					setCoordinates();
					checkCoordinates();
					paintArea(points[UPPER_X], points[UPPER_Y], points[LOWER_X], points[LOWER_Y]);
					painting = 1;
				}
			}
			state = 1;
		}
	}
}
