/**
 *	\file kc.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#ifndef _KC_H_
#define _KC_H_

#include "types.h"

#define WHITE_TXT 0x07 /* Atributo de video. Letras blancas, fondo negro */

/* Muestra la imagen de inicio */
/**
 * \fn retType function(args)
 *
 * 		\brief Brief.
 *
 * 		\param parameter ParamBrief.
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
void showSplashScreen();

/* Tiempo de espera */
/*void wait(int time);*/

/* Limpia la pantalla */
/**
 * \fn void k_clear_screen()
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
void k_clear_screen();

/* Inicializa la entrada del IDT */
/**
 * \fn void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access, byte cero)
 *
 * 		\brief Brief.
 *
 * 		\param item ParamBrief.
 * 		\param selector ParamBrief.
 * 		\param offset ParamBrief.
 * 		\param access ParamBrief.
 * 		\param cero ParamBrief.
 * 		
 * 		Use:
 * 		\code
 *		
 *		\endcode
 *
 * 		\sa f1() f2()
 *
 */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access, byte cero);

#endif
