/**
 *	\file system.h
 *	
 *		\brief  
 *
 *		\author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define STDIN 	0
#define STDOUT 	1
#define IN_ATT	2
#define OUT_ATT 3
#define LIVE 1
#define DEAD 0

enum PROCCESS { TERMINAL, SHELL, SCREEN_SAVER };

typedef int (*proccessT)(void);

/**
 *	\fn void rebootSystem(void)
 *
 *		\brief Reboots the system
 *
 */
void rebootSystem(void);

/**
 *	\fn void launchApp (int app)
 *  	\brief This function sets the the desired application to be run after the moment it is called.
 *      
 * 	\param app The desired application to be run.
 *      
 * 	\code
 *          launchApp(SCREEN_SAVER);
 *	\endcode
 * 	
 * 	\sa runApp() isAppRunning()
 */
void launchApp ( int app );

/**
 *	\fn void runApp (void)
 *  	\brief This function runs the application setted by launchApp().
 *      
 * 	\code
 *          runApp();
 *	\endcode
 * 	
 * 	\sa launchApp() isAppRunning()
 */
void runApp (void);

/**
 *	\fn void isAppRunning (int app)
 *  	\brief This function checks wether the given function is actually running or not.
 *      
 * 	\param app The desired application to be checked.
 * 	
 * 	\return TRUE if the application is currently running, FALSE if not.
 *      
 * 	\code
 *          int isTerminalRunning = isAppRunning(TERMINAL);
 *	\endcode
 * 	
 * 	\sa runApp() launchApp()
 */
int isAppRunning (int app);

/**
 * \fn void wait ( int miliseconds )
 *
 * 		\brief Stops the the execution for n miliseconds.
 *
 */
void wait ( int miliseconds );

#endif
