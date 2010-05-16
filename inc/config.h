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
 *		\brief DefineBrief.
 */
#define SCREEN_WIDTH 80
/**
 * \def SCREEN_HEIGHT
 *		\brief DefineBrief.
 */
#define	SCREEN_HEIGTH 25
/**
 * \def SCREEN_SIZE
 *		\brief DefineBrief.
 */
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGTH
/**
 * \def VIDEO_MEMORY_SIZE
 *		\brief DefineBrief.
 */
#define VIDEO_MEMORY_SIZE SCREEN_SIZE * 2
/**
 * \def CURSOR_START_COL
 *		\brief DefineBrief.
 */
#define CURSOR_START_COL	0
/**
 * \def CURSOR_START_ROW
 *		\brief DefineBrief.
 */
#define CURSOR_START_ROW	0
/**
 * \def VIDEO_TAB_STOP
 *		\brief DefineBrief.
 */
#define VIDEO_TAB_STOP 4
/**
 * \def VIDEO_VTAB_STOP
 *		\brief DefineBrief.
 */
#define VIDEO_VTAB_STOP 4
/**
 * \def CURSOR_START_VISIBLE
 *		\brief DefineBrief.
 */
#define CURSOR_START_VISIBLE 1
/**
 * \def VIDEO_COLOR
 *		\brief DefineBrief.
 */
#define VIDEO_COLOR setColor(BLACK, WHITE)
/**
 * \def KEYBOARD_REPETITION
 *		\brief DefineBrief.
 */
#define KEYBOARD_REPETITION		3
/**
 * \def ENABLED
 *		\brief DefineBrief.
 */
#define ENABLED 1
/**
 * \def DISABLED
 *		\brief DefineBrief.
 */
#define DISABLED 0
/**
 * \def MILISECONDS_PER_TICK
 *		\brief DefineBrief.
 */
#define MILISECONDS_PER_TICK 55
/**
 * \def TICKS_PER_SECOND
 *		\brief DefineBrief.
 */
#define TICKS_PER_SECOND 18
/**
 * \def STD_TTY
 *		\brief DefineBrief.
 */
#define STD_TTY 0
/**
 * \def MAX_PROCESS
 *		\brief DefineBrief.
 */
#define MAX_PROCESS			32
/**
 * \def MAX_CHILDS
 *		\brief DefineBrief.
 */
#define MAX_CHILDS			MAX_PROCESS - 1
/**
 * \def MAX_PROCESS_NAME
 *		\brief DefineBrief.
 */
#define MAX_PROCESS_NAME	36
/**
 * \def MAX_PRIORITIES
 *		\brief DefineBrief.
 */
#define MAX_PRIORITIES		4
/**
 * \def MAX_TTY
 *		\brief DefineBrief.
 */
#define MAX_TTY				8
/**
 * \def MAX_FILES
 *		\brief DefineBrief.
 */
#define MAX_FILES			2
/**
 * \def PAGES_PER_FRAME
 *		\brief DefineBrief.
 */
#define PAGES_PER_FRAME		8

/* Kernel Heap Area */
/**
 * \def KERNEL_HEAP_PAGES_QTY
 *		\brief DefineBrief.
 */
#define KERNEL_HEAP_PAGES_QTY		10

/*
 * Shell
*/
/**
 * \def MAX_LINE
 *		\brief DefineBrief.
 */
#define MAX_LINE 100

/**
 * \def ALARM
 *		\brief DefineBrief.
 */
#define	ALARM		'\a'
/**
 * \def BACKSPACE
 *		\brief DefineBrief.
 */
#define BACKSPACE	'\b'
/**
 * \def LINE_FEED
 *		\brief DefineBrief.
 */
#define LINE_FEED	'\f'
/**
 * \def NEW_LINE
 *		\brief DefineBrief.
 */
#define NEW_LINE	'\n'
/**
 * \def RETURN
 *		\brief DefineBrief.
 */
#define	RETURN		'\r'
/**
 * \def TAB
 *		\brief DefineBrief.
 */
#define TAB			'\t'
/**
 * \def VTAB
 *		\brief DefineBrief.
 */
#define VTAB		'\v'

#endif
