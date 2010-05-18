/**
 *	\file kasm.h
 *
 *		\brief Kernel utilities.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _KASM_H_
#define _KASM_H_

#include "types.h"

/**
 * \fn unsigned int _read_msw()
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
unsigned int _read_msw();

/**
 * \fn void _lidt (IDTR *idtr)
 *
 * 		\brief Brief.
 *
 * 		\param idtr ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void _lidt (IDTR *idtr);

/**
 * \fn void _mascaraPIC1 (byte mascara)
 *
 * 		\brief Brief.
 *
 * 		\param mascara ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void _mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */

/**
 * \fn void _mascaraPIC2 (byte mascara)
 *
 * 		\brief Brief.
 *
 * 		\param mascara ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void _mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

/**
 * \fn void _Cli(void)
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
void _Cli(void);        /* Deshabilita interrupciones  */

/**
 * \fn void _Sti(void)
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
void _Sti(void);	 /* Habilita interrupciones  */

/**
 * \fn void _debug (void)
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
void _debug (void);

/**
 * \fn unsigned getCodeSegment()
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
unsigned getCodeSegment();

#endif