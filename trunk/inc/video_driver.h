/**
 *	\file video_driver.h
 *
 *		\brief Defines the basic function to manage the video screen, cursor &  mouse
 *		pointer.
 *
 *		\author Guido Marucci Blas, Nicolas Purita, Luciano Zemin
 *
 */

#ifndef VIDEO_DRIVER_H_
#define VIDEO_DRIVER_H_

/*
 *	Special Characters Definition
 */

/**
 *	\fn void refreshScreen ( void )
 *	
 *		\brief Updates all changes made to video buffer into screen
 *
 */
void refreshScreen ( void );


/**
 * \fn void initVideo ( int cursorEnabled, int pointerEnabled )
 *
 *		\brief Initializes the video driver, by setting the initial
 *		cursor, pointer position, setting their visibility, enabling
 *		the blinking cursor and defining cursor shape.
 *
 *		\param cursorEnabled zero to disable the cursor, not zero to enable the cursor.
 *		\param pointerEnabled zero to disable the cursor, not zero to enable the cursor.
 */
void initVideo ( int cursorEnabled, int pointerEnabled );

/**
 * \fn void incCursor ( void )
 *
 * 		\brief Increments the position of the cursor by one unit.
 * 		Automatically resolves new lines and screen scrolling.
 *
 */
void incCursor ( void );

/**
 *	\fn void decCursor( void )
 *
 *		\brief Decrements the position of the cursor by one unit.
 *		Automatically resolves jumps to previous lines.
 *
 */
void decCursor( void );

/**
 * \fn int setCursorVisibility ( int b )
 *
 * 		\brief Sets the cursor's visibility.
 * 		Zero to turn it off not zero to turn it on.
 *
 * 		\return The actual state of visibility
 *
 */
int setCursorVisibility ( int b );

/**
 *	\fn int setCursorPosition ( int row, int col )
 *
 *		\brief Sets the cursor's position.
 *
 *		\param row the new row position.
 *		\param col the new col position.
 *
 *		\return the offset from the (0,0) including
 *		the attribute byte.
 *
 */
int setCursorPosition ( int row, int col );

/**
 * 	\fn int setCursorPositionByOffset ( int offset )
 *
 * 		\brief Sets the cursor's position
 *
 * 		\param offset the offset from the (0,0) including
 * 		the attribute byte.
 *
 */
int setCursorPositionByOffset ( int offset );


/**
 *	\fn void getCursorPosition ( int * row, int * col )
 *
 * 		\brief Gets the actual cursor's position.
 *
 * 		\param row a pointer to int where the actual row position will be returned. Must not be NULL.
 * 		\param col a pointer to int where the acutal col position will bie returned. Must not be NULL.
 *
 */
void getCursorPosition ( int * row, int * col );

/**
 *	\fn int setPointerPosition ( int row, int col )
 *
 *		\brief Sets the mouse pointer's position.
 *
 *		\param row the new row position.
 *		\param col the new col position.
 *
 *		\return the offset from (0, 0) incliding the attribute byte.
 *
 */
int setPointerPosition ( int row, int col );


/**
 *	\fn int setPointerPositionByOffset ( int offset )
 *
 *		\brief Sets the pointer's position.
 *
 *		\param offset the offset from (0,0) including the attribute byte.
 *
 */
int setPointerPositionByOffset ( int offset );


/**
 *	\fn int getPointerPosition ( int * row, int * col )
 *
 *		\brief Gets the pointer's position.
 *
 *		\row a pointer to int where the actual row position will be returned. Must not be NULL.
 *		\col a pointer to int where the actual col position will be returned. Must not be NLL.
 *
 *		\return the offset from (0,0) including the attribute offset.
 *
 */
int getPointerPosition ( int * row, int * col );

/**
 *	\fn int setPointerVisibility ( int b )
 *
 *		\brief Sets the pointer's visibility
 *
 *		\param b zero to turn it off, not zero to turn it on.
 *
 *		\return the actual state of visibility.
 *
 */
int setPointerVisibility ( int b );

/**
 *	\fn int isPointerVisible ( void )
 *
 *		\brief Gets the acutal state of visibility
 *
 *		\return the actul state of visibility.
 *
 */
int isPointerVisible ( void );


/**
 * \fn int getPointerColor ( void )
 *
 * 		\brief Gets the poiter's color
 *
 * 		\return A color code.
 *
 */
int getPointerColor ( void );

/**
 *	\fn int setPointerColor ( int color )
 *
 * 		\brief Sets the pointer's color
 *
 * 		\param color A color code.
 * 		\return the new pointer's color code.
 *
 */
int setPointerColor ( int color );

/**
 *	\fn int getVideoColor ( void )
 *
 *		\brief Gets the actual default video color
 *
 * 		\return the actual video color code
 *
 */
int getVideoColor ( void );


/**
 *	\fn int setVideoColor ( int color )
 *
 * 		\brief Set the default video color
 *
 * 		\param color the new video color code
 *		\return the new video color code
 *
 */
int setVideoColor ( int color );


/**
 *	\fn int paintArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y )
 *
 *		\brief Paints an area delimited by two opossite points by changing the background color
 *		with the font color.
 *
 *		\return A number different from zero if succed zero if not.
 *
 */
int paintArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y );


/**
 *	\fn int copyArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y, char * buffer )
 *
 *		\brief Copies an area delimited by two opposite points including the attribute byte into a buffer.
 *
 *
 *		\return A number different from zero if succed zero if not.
 *
 */
int copyArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y, char * buffer );

/**
 *	\fn int pasteArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y, char * buffer )
 *
 *		\brief Pastes a buffer into an area delimited by two opposite points including the attribute byte.
 *
 *		\return A number different from zero if succed zero if not.
 *
 */
int pasteArea ( int leftUpper_x, int leftUpper_y, int rightLower_x, int rightLower_y, char * buffer );

/**
 *	\fn void putCharAtCurrentPos ( int c, int color )
 *
 *		\brief Puts a character into the video buffer at the current cursor position
 *
 *		\param c the character to be putted into the video buffer
 *		\param color the color code of the character
 *
 *
 */
void putCharAtCurrentPos ( int c, int color );


/**
 *  \fn void printChar ( int c )
 *
 * 		\brief Puts a character into the video buffer at the current position and moves the cursor.
 *
 * 		\param c the character to be putted into the video buffer
 *
 */
void printChar ( int c );


/**
 *	\fn void printColorChar ( int c, int color )
 *
 * 		\brief Puts a character into the video buffer at the current position and moves the cursor.
 * 		
 * 		\param c the character to be putted into the video buffer
 * 		\parma color the color code of the character.
 *
 */
void printColorChar ( int c, int color );

/**
 *	\fn int putCharAtFixedPos ( int c, int color, int row, int col )
 *
 *		\brief Puts a character into the video buffer at a given position.
 *
 * 		\param c the character to be putted into the video buffer
 * 		\parma color the color code of the character.
 *		\param row the row position where the character will be putted
 *		\param col the col position where the character will be putted
 *
 *
 */
int putCharAtFixedPos ( int c, int color, int row, int col );


/**
 *	\fn int putColoredStringAtFixedPos ( char * s, int * color, int length, int row, int col  )
 *
 *
 *		\brief Puts a string at a given position.
 *		
 *		\param s The string to be putted.
 *		\param color a pointer to int with the color codes for each character or NULL to print with the default video color.
 *
 *
 */
int putColoredStringAtFixedPos ( char * s, int * color, int length, int row, int col  );


/**
 *	\fn void fillScreen ( int c )
 *
 *		\brief Fills the screen with a character
 *
 *
 */
void fillScreen ( int c );


/**
 * \fn void clearScreen ( void )
 *
 * 		Clears the screen
 *
 */
void clearScreen ( void );

/**
 *	\fn int getVideoTabStop ( void )
 *
 *		\brief Gets the video tab stop
 *		
 */
int getVideoTabStop ( void );

/**
 *	\fn int setVideoTabStop ( int num )
 *
 * 		\brief Sets the video tab stop.
 *
 */
int setVideoTabStop ( int num ); 

/**
 *	\fn int getVideoVTabStop ( void )
 *
 *		\brief Gets the video vertical tab.
 *
 */
int getVideoVTabStop ( void );

/**
 * 	\fn int setVideoVTabStop ( int num )
 *
 * 		\brief Sets the video vertical tab stop
 *
 */
int setVideoVTabStop ( int num );


/**
 *	\fn void updateColor ( void  )
 *
 *		\brief Updates the hole screen with actual video color
 *		all colors will be changed.
 *
 */
void updateColor ( void  );

#endif
