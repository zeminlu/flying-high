/* 
 * File: videoDriverRef.h
 * Author: Santiago Bermudez
 * Version: 02/06/2009 - 12:17
 * Description: The video controller manages all operations in the screen.
 * Provides management functions of the mouse, screen printing and storage
 * areas of the screen.
 */

#ifndef VIDEODRIVERREF_H_
#define VIDEODRIVERREF_H_


/* 
 * Include project section 
 */

#include "stdio.h"

/* 
 * Define section
 */
#define SCREEN_SIZE 80*25
#define NO_MOUSE_FORMAT (BLACK_B | WHITE_F)
/* 
 * The following table represents the Foregraunds(F) and Backgrounds(B) colors.
 * A format is a disjunction of a foregrand and a background, and eventually,
 * the blinking.
 */

#define BLACK_F			0x0
#define BLUE_F			0x1
#define GREEN_F			0x2
#define CYAN_F			0x3
#define RED_F			0x4
#define MAGENTA_F		0x5
#define BROWN_F			0x6
#define LIGHT_GRAY_F		0x7
#define DARK_GRAY_F		0x8
#define LIGHT_BLUE_F		0x9
#define LIGHT_GREEN_F		0xA
#define LIGHT_CYAN_F		0xB
#define LIGHT_RED_F		0xC
#define LIGHT_MAGENTA_F		0xD
#define YELLOW_F		0xE
#define WHITE_F			0xF
#define BLACK_B			0x00
#define BLUE_B			0x10
#define GREEN_B			0x20
#define CYAN_B			0x30
#define RED_B			0x40
#define MAGENTA_B		0x50
#define BROWN_B 		0x60
#define LIGHT_GRAY_B		0x70
#define BLINKING		0x80

/*
 * function: saveScrren
 * Description: Saves the actual state of the screen in a buffer. It can
 * 		be restored useing restoreScreen fuction
 *
 */

void saveScreen( int );

/*
 * function: restoreScreen
 * Description: restores the screen from a buffer written by
 * 		saveScreen function
 *
 */

void restoreScreen( void );

/*
 * Function: initializeScreen
 * Description: Initialize the screen, set the left limit, forbidding the erase 
 * before this limit.
 * Parameters:
 * 	leftBound: The left limit.
 * 	defaultFormat: The starting attribute.
 */

void initializeScreen( int leftBound, unsigned char defaultFormat );
/*
 * Section: Mouse management.
 * Description: Provides all functions to manage the mouse in screen.
 */

/*
 * Function: turnOnMouse
 * Description: Turn on the mouse on screen. Set the format of the mouse's
 * cursor position. Initialize the cursor on screen's center.
 * Parameters:
 * 	mouseFormat: A format defined by the table formats.
 */

void turnOnMouse( unsigned char mouseFormat );

/*
 * Function: mouseSet
 * Description: Moves the mouse's cursor on screen. If the movement is out of bounds,
 * puts the cursor in the correspondant limit.
 * Parameters:
 * 	dx: Movement on X axis.
 * 	dy: Movement on Y axis.
 */

void mouseSet( int dx, int dy);

/*
 * Function: getMouseX
 * Description: Returns cursor's position on X axis.
 */

int getMouseX( void );

/*
 * Function: getMouseY
 * Description: Returns cursor's position on Y axis.
 */

int getMouseY( void );

/*
 * Function: changeMouseFormat
 * Description: Set the cursor format.
 * Parameters:
 * 	mouseFormat: A format defined by the table formats.	
 */

void changeMouseFormat( unsigned char mouseFormat );

/*
 * Section: Storage management.
 * Description: Provides functions to bufferize screen areas.
 */


/*
 * Function: bufferizeScreen
 * Description: Writes in buffer the area framed.
 * Prameters:
 * 	lX: Lower X.
 * 	uX: Upper X.
 * 	lY: Lower Y.
 * 	uY: Upper Y.
 * 	buffer: Where the framed area is copied. 
 */ 

size_t bufferizeScreen( int lX, int uX, int lY, int uY, unsigned char * buffer );

/*
 * Function: enterOnScreen
 * Description: Writes an enter on screen.
 */

void enterOnScreen( int fd );

/*
 * Function: backSpaceOnScreen
 * Description: Writes a backSpace on screen.
 */

int backSpaceOnScreen( int fd );

/*
 * Section: Screen printing.
 * Description: Provides functions to print on screen.
 */ 

/*
 * Function: videoDriverWrite
 * Description: Writes on the file descriptor an amount count, starting on 
 * the buffer.
 * Parameters:
 * 	fileDescriptor: A file descriptor.
 * 	buffer: Where data starts.
 * 	count: The amount to write.
 */

ssize_t videoDriverWrite( int fileDescriptor, const void * buffer, size_t count );

/*
 * Function: videoDriverWriteAt
 * Description: same as videoDriveWrite but writes with atrubutes.
 * 
 * Parameters:
 * 	fileDescriptor: A file descriptor.
 * 	buffer: Where data starts.
 * 	count: The amount to write.
 */

ssize_t videoDriverWriteAt( int fileDescriptor, const void * buffer, size_t count );

/*
 * Function: screenColorArea
 * Description: Sets the format of framed area.
 * Prameters:
 * 	lX: Lower X.
 * 	uX: Upper X.
 * 	lY: Lower Y.
 * 	uY: Upper Y.
 * 	format: A format defined by the table formats.
 */ 

void screenColorArea( int lX, int uX, int lY, int uY, unsigned char format );

/*
 * Function: screenColor
 * Description: Sets the format on the screen.
 * Prameters:
 * 	format: A format defined by the table formats.
 */ 

void screenColor( unsigned char format );

/*
 * Function: clearScreen
 * Description: clear the screen leaving only the
 * 				shell on the top.
 *
 */

void clearScreen( int );

#endif
