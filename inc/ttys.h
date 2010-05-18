/**
 *	\file ttys.h
 *
 *		\brief The tty header file.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#ifndef _TTYS_H_
#define _TTYS_H_

#include "queue.h"
#include "video_driver.h"
#include "string.h"
#include "keyboard_buffer.h"
#include "sysProcess.h"
#include "syscall.h"
#include "types.h"
#include "unistd.h"
#include "kMalloc.h"

/**
 * \fn void getReadPointer(tty_t tty, int *readPointer, int *readCol, int *readRow)
 *
 * 		\brief This function obtains read pointers of the tty.
 *
 * 		\param tty The tty where the function obtain the pointers
 * 		\param readPointer The position of the pointer where it is reading
 * 		\param readCol The col position of the stdout where it is reading
 * 		\param readRow The row position of the stdout where it is reading
 * 		
 */
void getReadPointer(tty_t tty, int *readPointer, int *readCol, int *readRow);

/**
 * \fn void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow)
 *
 * 		\brief This function obtains writer pointers of the tty.
 *
 * 		\param tty The tty where the function obtain the pointers
 * 		\param writePointer The position of the pointer where it is writting
 * 		\param writeCol The col position of the stdout where it is writting
 * 		\param writeRow The row position of the stdout where it is writting
 *
 */
void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow);

/**
 * \fn void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow)
 *
 * 		\brief This function saves the reads pointers of the tty that is indicated
 *				in the signature. This function is called when change the tty that
 *				is in focus, so it has to backup it's pointer
 *
 * 		\param tty The tty where the function save the pointers
 * 		\param readPointer The position of the pointer where it is reading
 * 		\param readCol The col position of the stdout where it is reading
 * 		\param readRow The row position of the stdout where it is reading
 *
 *
 */
void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow);

/**
 * \fn void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow)
 *
 * 		\brief This function saves the reads pointers of the tty that is indicated
 *				in the signature. This function is called when change the tty who
 *				is in focus, so it has to backup his pointer
 *
 * 		\param tty The tty where the function save the pointers
 * 		\param writePointer The position of the pointer where it is writting
 * 		\param writeCol The col position of the stdout where it is writting
 * 		\param writeRow The row position of the stdout where it is writting
 *
 */
void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow);

/**
 * \fn void putsTTY( unsigned char *name, int count, tty_t tty )
 *
 * 		\brief This function prints a string in a specific tty.
 *				It checks if it has to print only in the tty or in stdin too.
 *				It called to putCharTTy while the length of the string was grater
 *				than 0.
 *
 * 		\param name The string to print
 * 		\param count The size of the string
 * 		\param tty The tty id where has to be printed the string
 * 		
 * 		Use:
 * 		\code
 *			char *string = "test";
 *			...
 *			putsTTY(string);
 *			...
 *		\endcode
 *
 * 		\sa putCharTTY
 *
 */
void putsTTY( unsigned char *name, int count, tty_t tty );

/**
 * \fn void putCharTTY( char c, tty_t tty, int inStdIn )
 *
 * 		\brief It prints only one character in specific tty.
 *				It verifies if the character is a control key or
 *				it belongs to the ASCII table. It checks if the
 *				tty is what are in focus, so it has to put the 
 *				character in stdin buffer.
 *
 * 		\param c The chacter to be printed
 * 		\param tty The tty id where has to be printed
 * 		\param inStdIn If it has to print in Stdin or not
 * 		
 * 		Use:
 * 		\code
 *			char *string = "System";
 *			while( *string != NULL )
 *				putCharTTY(*string++, tty, (TRUE v FALSE) ):
 *		\endcode
 *
 *
 */
void putCharTTY( char c, tty_t tty, int inStdIn );

/**
 * \fn void initializeTTY( void )
 *
 * 		\brief Create the table of ttys. The maximum of ttys
 *				is a define.
 *
 * 		Use:
 * 		\code
 *			...
 *			initializeTTY();
 *			...
 *		\endcode
 *
 */
void initializeTTY( void );

/**
 * \fn int getFocusedTTY( void )
 *
 * 		\brief This function returns the tty id of tty that are
 *				in focus at this moment. It is a system called.
 *
 * 		\return The id of tty that is in focus
 *
 * 		Use:
 * 		\code
 *			int tty;
 *			tty = getFocusedTTY();
 *			putCharTTY( 'a', tty, TRUE );
 *			...
 *		\endcode
 *
 */
int getFocusedTTY( void );

/**
 * \fn int changeFocusTTY( tty_t nextTty )
 *
 * 		\brief This function changes the tty focus id on
 *				the next Tty that has to be in focus. This function
 *				is called when the key that was pressed is between
 *				"F1" and "F7". Of every "F" key that was pressed, is put
 *				a value between 0x81 and 0x87. The number of the "F" key
 *				+ 0x80
 *
 * 		\param nextTty Next tty id
 * 		
 * 		\return 1 if the next tty id is the current tty id in focus, 0 if it
 *				has changed the focus.
 *
 * 		Use:
 * 		\code
 *			unsigned int c;
 *			while( !isEmptyKBBuffer() ){
 *				c = deQueue();
 *				...
 *				if ( c >= 0x81 && c <= 0x87 )
 *					changeFocusTTY( c - 0x81 );
 *				
 *		\endcode
 *
 */
int changeFocusTTY( tty_t nextTty );

/**
 * \fn void refreshStdout(void)
 *
 * 		\brief Pust all of character that are in the STDOUT and
 *			and puts in the tty STDOUT who is in focus.
 *
 * 		Use:
 * 		\code
 *			
 *		\endcode
 *
 */
void refreshStdout(void);

/**
 * \fn void refreshKeyboardBufferTTY( void )
 *
 * 		\brief Leaves all of character that are in the KeyboradBuffer
 *				and put in the correct tty stdout if the tty mode is
 *				CANONICAL, if not it prints directly in the STDIN of
 *				tty who is in focus. The timet tick is the responsible to
 *				call this function.
 *
 * 		Use:
 * 		\code
 *			...
 *			call Scheculer
 *			call refreshKeyboardBufferTTY
 *			...
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void refreshKeyboardBufferTTY( void );

/**
 * \fn void refreshTTY(void)
 *
 * 		\brief This functions check if the proccess if different of init
 *				and the running process has a valid pid, it only refreshes
 *				the stdout.
 *
 */
void refreshTTY(void);
/**
 * \fn void sysSetTTYFocusedProcess(pid_t pid, tty_t tty)
 *
 * 		\brief Sets the pid to the process which is in focus
 *
 * 		\param pid The pid of the process
 * 		\param tty The tty where will be set the pid of the process
 * 		
 *
 */
void sysSetTTYFocusedProcess(pid_t pid, tty_t tty);

/**
 * \fn pid_t sysGetTTYFocusedProcess(tty_t tty)
 *
 * 		\brief Returns the pid of the process that is in this tty
 *
 * 		\param tty The tty where it is saved the pid of the focus process
 * 		
 *
 */
pid_t sysGetTTYFocusedProcess(tty_t tty);

/**
 * \fn void clearTTYScreen()
 *
 * 		\brief It clears the stdout of the tty that is in focus
 *
 */
void clearTTYScreen();

/**
 * \fn void setTTYCursorPosition( int x, int y)
 *
 * 		\brief It sets the cursor in an specific position of the
 *				tty stdout.
 *
 * 		\param x The row of the stdout
 * 		\param y The column of the stdout
 * 		
 *
 */
void setTTYCursorPosition( int x, int y);

/**
 * \fn void putTTYCharAtPosition( int c, int row, int col)
 *
 * 		\brief It puts a character in a specific position of the
 *				tty stdout. It prints the character at the position
 *				but it keeps the previous position.
 *
 * 		\param c The character to be printed
 * 		\param row The row where has to be printed
 * 		\param col The column where has to be printed
 * 		
 *
 */
void putTTYCharAtPosition( int c, int row, int col);

#endif