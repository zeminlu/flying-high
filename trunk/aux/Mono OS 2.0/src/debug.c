/*
 *	debug.c
 *
 *		Implements debug.h
 *
 * 		Author: Guido Marucci Blas
 */
#include "debug.h"
#include "stdio.h"
#include "kasm.h"

#ifdef DEBUG_LEVEL
static int debug = DEBUG_LEVEL;
#else
static int debug = NO_DEBUG;
#endif

#ifdef DEBUG_PAUSE
static int debugPause = DEBUG_PAUSE;
#else
static int debugPause = NO_DEBUG;
#endif

void str_debug(int debugLevel, const char *function, char *message, int newLine) {
	
	if ( debug == NO_DEBUG || (debug != DEBUG_ALL_LEVELS && debugLevel != debug) )
		return;

	if ( function != NULL ) {
		puts((const unsigned char *)"[debug]: ");
		puts((const unsigned char *)function);
		puts((const unsigned char *)": ");
	}
	puts((const unsigned char *)message);
	if ( newLine )
		puts((const unsigned char *)"\n");
	
	return;
}

void int_debug(int debugLevel, const char *function, int message, int newLine) {
	
	if ( debug == NO_DEBUG || (debug != DEBUG_ALL_LEVELS && debugLevel != debug) )
		return;

	if ( function != NULL ) {
		puts((const unsigned char *)"[debug]: ");
		puts((const unsigned char *)function);
		puts((const unsigned char *)": ");
	}
	puti(message);
	if ( newLine )
		puts((const unsigned char *)"\n");
	
	return;
}

void hexa_debug(int debugLevel, const char *function, int message, int newLine) {

	if ( debug == NO_DEBUG || (debug != DEBUG_ALL_LEVELS && debugLevel != debug) )
		return;

	if ( function != NULL ) {
		puts((const unsigned char *)"[debug]: ");
		puts((const unsigned char *)function);
		puts((const unsigned char *)": ");
	}
	putx(message);
	if ( newLine )
		puts((const unsigned char *)"\n");
	
	return;
}

void pause_debug(int debugLevel) {
	if ( debugPause == NO_DEBUG || (debugPause != DEBUG_ALL_LEVELS && debugLevel != debugPause) )
		return;
	puts((const unsigned char *)"\n[debug]: Execution paused. Set register ax=0 to continue.\n");
	_debug();
	return;
}
