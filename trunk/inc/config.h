/**
 * \file config.h
 *
 * 		\brief OS configuration defines
 *
 * 		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "colors.h"

/*
 *	System Properties
 */

/**
 * \def SCREEN_WIDTH
 *		\brief The width of the screen
 */
#define SCREEN_WIDTH 80
/**
 * \def SCREEN_HEIGTH
 *		\brief The height of the screen
 */
#define	SCREEN_HEIGTH 25
/**
 * \def SCREEN_SIZE
 *		\brief Real screen size without color attribute
 */
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGTH
/**
 * \def VIDEO_MEMORY_SIZE
 *		\brief Real screen size with color attributes
 */
#define VIDEO_MEMORY_SIZE SCREEN_SIZE * 2
/**
 * \def CURSOR_START_COL
 *		\brief The starter column position
 */
#define CURSOR_START_COL	0
/**
 * \def CURSOR_START_ROW
 *		\brief The starter row position
 */
#define CURSOR_START_ROW	0
/**
 * \def VIDEO_TAB_STOP
 *		\brief The spaces that tab prints
 */
#define VIDEO_TAB_STOP 4
/**
 * \def VIDEO_VTAB_STOP
 *		\brief The vertical tab spaces
 */
#define VIDEO_VTAB_STOP 4
/**
 * \def CURSOR_START_VISIBLE
 *		\brief The state of the cursor, Visible = 1 v NotVisible = 0
 */
#define CURSOR_START_VISIBLE 1
/**
 * \def VIDEO_COLOR
 *		\brief Set color of letters and screen
 */
#define VIDEO_COLOR setColor(BLACK, WHITE)
/**
 * \def KEYBOARD_REPETITION
 *		\brief The amount of key press repetitions
 */
#define KEYBOARD_REPETITION		3
/**
 * \def ENABLED
 *		\brief Enabled cursor
 */
#define ENABLED 1
/**
 * \def DISABLED
 *		\brief Disabled cursor
 */
#define DISABLED 0
/**
 * \def MILISECONDS_PER_TICK
 *		\brief The amount of miliseconds per tick
 */
#define MILISECONDS_PER_TICK 55
/**
 * \def TICKS_PER_SECOND
 *		\brief The amount of ticks in one second
 */
#define TICKS_PER_SECOND 18
/**
 * \def STD_TTY
 *		\brief
 */
#define STD_TTY 0
/**
 * \def MAX_PROCESS
 *		\brief The maximum amount of process
 */
#define MAX_PROCESS			32
/**
 * \def MAX_CHILDS
 *		\brief The maximum amount of childs that a process could have.
 */
#define MAX_CHILDS			MAX_PROCESS - 1
/**
 * \def MAX_PROCESS_NAME
 *		\brief The maximum length of process name
 */
#define MAX_PROCESS_NAME	36
/**
 * \def MAX_PRIORITIES
 *		\brief The amount of priority that the system accept
 */
#define MAX_PRIORITIES		4
/**
 * \def MAX_TTY
 *		\brief The maximum amount of ttys
 */
#define MAX_TTY				8
/**
 * \def MAX_FILES
 *		\brief The maximum amount of files
 */
#define MAX_FILES			2
/**
 * \def PAGES_PER_FRAME
 *		\brief The amount of pages per frame
 */
#define PAGES_PER_FRAME		8

/* Kernel Heap Area */
/**
 * \def KERNEL_HEAP_PAGES_QTY
 *		\brief The amount of pages that the kernel has in his heap
 */
#define KERNEL_HEAP_PAGES_QTY		10

/*
 * Shell
*/
/**
 * \def MAX_LINE
 *		\brief The length of the shell buffer
 */
#define MAX_LINE 100

/**
 * \def ALARM
 *		\brief The alarm control ascii value
 */
#define	ALARM		'\a'
/**
 * \def BACKSPACE
 *		\brief The backspace control ascii value
 */
#define BACKSPACE	'\b'
/**
 * \def LINE_FEED
 *		\brief The line feed control ascii value
 */
#define LINE_FEED	'\f'
/**
 * \def NEW_LINE
 *		\brief The new line control ascii value
 */
#define NEW_LINE	'\n'
/**
 * \def RETURN
 *		\brief The return control ascii value
 */
#define	RETURN		'\r'
/**
 * \def TAB
 *		\brief The tabular control ascii value
 */
#define TAB			'\t'
/**
 * \def VTAB
 *		\brief The vertical tab control ascii value.
 */
#define VTAB		'\v'

#endif
