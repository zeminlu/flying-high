/**
 *	\file colors.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _COLORS_H_
#define _COLORS_H_

#define BLACK			0x0
#define BLUE			0x1
#define GREEN			0x2
#define CYAN			0x3
#define	RED				0x4
#define MAGENTA 		0x5
#define BROWN			0x6
#define LIGHT_GREY		0x7
#define DARK_GREY		0x8
#define LIGHT_BLUE		0x9
#define LIGHT_GREEN		0xA
#define LIGHT_CYAN		0xB
#define LIGHT_RED		0xC
#define LIGHT_MAGENTA	0xD
#define LIGHT_BROWN		0xE
#define WHITE			0xF

#define setColor(bkg,text) (((bkg) << 4) + (text)) 

#endif
