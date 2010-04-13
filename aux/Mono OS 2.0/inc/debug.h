/*
 *	debug.h
 *
 * 		Defines function to debug printing messages
 *		
 *		Author: Guido Marucci Blas
 *
 */
#ifndef DEBUG_H_
#define DEBUG_H_

#define NO_DEBUG			-1
#define DEBUG_ALL_LEVELS	0
#define MEM_DEBUG			1
#define FILESYS_DEBUG		2
#define MTASK_DEBUG			3
#define PHEAP_DEBUG			666
#define BASE_MALLOC_DEBUG	5
#define WAIT_TTY_DEBUG   	6
/*
 *	@fn str_debug(char *function, char *message, int newLine)
 *
 *		@brief Checks if the __DEBUG symbol is
 *		defined and prints the given message.
 *		@param function The caller function's name or NULL if no
 *		function name needs to be printed.
 *		@param message The message to be printed
 *		@param newLine TRUE if a new line needs to be printed. 
 */
void str_debug(int debugLevel, const char *function, char *message, int newLine);

/*
 *	@fn int_debug(char *function, char *message, int newLine)
 *
 *		@brief Checks if the __DEBUG symbol is
 *		defined and prints the given message.
 *		@param function The caller function's name or NULL if no
 *		function name needs to be printed.
 *		@param message The message to be printed
 *		@param newLine TRUE if a new line needs to be printed. 
 */
void int_debug(int debugLevel, const char *function, int message, int newLine);

/*
 *	@fn hexa_debug(char *function, char *message, int newLine)
 *
 *		@brief Checks if the __DEBUG symbol is
 *		defined and prints the given message in hexadecimal format.
 *		@param function The caller function's name or NULL if no
 *		function name needs to be printed.
 *		@param message The message to be printed
 *		@param newLine TRUE if a new line needs to be printed. 
 */
void hexa_debug(int debugLevel, const char *function, int message, int newLine);

/*
 *	@fn void pause_debug(int debugLevel)
 *
 *		@brief Holds the execution until reigster ax is set to zero, for
 *		the given debug level. The symbol DEBUG_PAUSE should be defined.
 *		@param debugLevel The debug level on wich the execution will be stopped.
 *
 */
void pause_debug(int debugLevel);

#endif 
