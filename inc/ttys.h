/**
 *	\file ttys.h
 *
 *		\brief Brief.
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
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
 * 		\param readPointer ParamBrief.
 * 		\param readCol ParamBrief.
 * 		\param readRow ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void getReadPointer(tty_t tty, int *readPointer, int *readCol, int *readRow);

/**
 * \fn void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow)
 *
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
 * 		\param writePointer ParamBrief.
 * 		\param writeCol ParamBrief.
 * 		\param writeRow ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void getWritePointer(tty_t tty, int *writePointer, int *writeCol, int *writeRow);

/**
 * \fn void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow)
 *
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
 * 		\param readPointer ParamBrief.
 * 		\param readCol ParamBrief.
 * 		\param readRow ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void setReadPointer(tty_t tty, int readPointer, int readCol, int readRow);

/**
 * \fn void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow)
 *
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
 * 		\param writePointer ParamBrief.
 * 		\param writeCol ParamBrief.
 * 		\param writeRow ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void setWritePointer(tty_t tty, int writePointer, int writeCol, int writeRow);

/**
 * \fn void putsTTY( unsigned char *name, int count, tty_t tty )
 *
 * 		\brief Brief.
 *
 * 		\param name ParamBrief.
 * 		\param count ParamBrief.
 * 		\param tty ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void putsTTY( unsigned char *name, int count, tty_t tty );

/**
 * \fn void putCharTTY( char c, tty_t tty, int inStdIn )
 *
 * 		\brief Brief.
 *
 * 		\param c ParamBrief.
 * 		\param tty ParamBrief.
 * 		\param inStdIn ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void putCharTTY( char c, tty_t tty, int inStdIn );

/**
 * \fn void initializeTTY( void )
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void initializeTTY( void );

/**
 * \fn int getFocusedTTY( void )
 *
 * 		\brief Brief.
 *
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int getFocusedTTY( void );

/**
 * \fn int changeFocusTTY( tty_t nextTty )
 *
 * 		\brief Brief.
 *
 * 		\param nextTty ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
int changeFocusTTY( tty_t nextTty );

/**
 * \fn void refreshStdout(void)
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void refreshStdout(void);

/**
 * \fn void refreshKeyboardBufferTTY( void )
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void refreshKeyboardBufferTTY( void );

/**
 * \fn void refreshTTY(void)
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void refreshTTY(void);

/**
 * \fn void sysPutS(Keycode *name, int count, tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param name ParamBrief.
 * 		\param count ParamBrief.
 * 		\param parameter ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void sysPutS(Keycode *name, int count, tty_t tty);

/**
 * \fn void sysPutChar(Keycode c, tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param c ParamBrief.
 * 		\param tty ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void sysPutChar(Keycode c, tty_t tty);

/**
 * \fn void sysSetTTYFocusedProcess(pid_t pid, tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param pid ParamBrief.
 * 		\param tty ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void sysSetTTYFocusedProcess(pid_t pid, tty_t tty);

/**
 * \fn pid_t sysGetTTYFocusedProcess(tty_t tty)
 *
 * 		\brief Brief.
 *
 * 		\param tty ParamBrief.
 * 		
 * 		\return Description.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
pid_t sysGetTTYFocusedProcess(tty_t tty);

/**
 * \fn void clearTTYScreen()
 *
 * 		\brief Brief.
 *
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void clearTTYScreen();

/**
 * \fn void setTTYCursorPosition( int x, int y)
 *
 * 		\brief Brief.
 *
 * 		\param x ParamBrief.
 * 		\param y ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void setTTYCursorPosition( int x, int y);

/**
 * \fn void putTTYCharAtPosition( int c, int row, int col)
 *
 * 		\brief Brief.
 *
 * 		\param c ParamBrief.
 * 		\param row ParamBrief.
 * 		\param col ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void putTTYCharAtPosition( int c, int row, int col);

#endif