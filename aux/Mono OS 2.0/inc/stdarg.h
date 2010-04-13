/*
 *	stdarg.h
 *		
 *
 *
 *		Author: Guido Marucci Blas
 */

#ifndef STDARG_H_
#define STDARG_H_

typedef void **va_list;

#define va_start(ap,last) 	(*(ap) = (last))
#define va_arg(ap,type)		( *((type )(*(ap))) )

void va_end( va_list ap);

#endif
