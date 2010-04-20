/**
 * \file config.h
 *
 * 		\brief 
 *
 * 		\author Guido Marucci Blas,  Nicolas Purita & Luciano Zemin
 *
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "colors.h"

/*
 *	System Properties
 */
#define SCREEN_WIDTH 80
#define	SCREEN_HEIGTH 25
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGTH
#define VIDEO_MEMORY_SIZE SCREEN_SIZE * 2
#define CURSOR_START_COL	0
#define CURSOR_START_ROW	0
#define POINTER_START_COL SCREEN_WIDTH / 2
#define POINTER_START_ROW SCREEN_HEIGTH / 2
#define VIDEO_TAB_STOP 4
#define VIDEO_VTAB_STOP 4
#define CURSOR_START_VISIBLE 1
#define POINTER_START_VISIBLE 1
#define POINTER_COLOR setColor(GREEN, GREEN)
#define VIDEO_COLOR setColor(BLACK, WHITE)
#define KEYBOARD_REPETITION		3
#define ENABLED 1
#define DISABLED 0
#define MILISECOND_PER_TICKS 55
#define TICKS_PER_SECOND 18
#define DEFAULT_SCREENSAVER_TIME 540

#define MAX_PROCESS			24
#define MAX_CHILDS			30
#define MAX_PROCESS_NAME	36
#define MAX_PRIORITIES		4
#define MAX_TTY				8

#endif
