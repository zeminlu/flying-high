/*
 *	File: convertions.c
 *	Author: Lucas Pizzagalli 
 *	Version: 24.05.09.04.47
 */


#include "string.h"

/*
 * Function: sprintint
 *
 * Description:
 * 		Converts a integer into a stringi(USED BY MYSPRINTF).
 * Input:
 * 		buff = buffer where integer will be written
 * 		lim = size of buff
 * 		num = integer wanted to be convert
 * Output:
 * 		number of chars written
 * Programmer:
 * 		ssp
 * Version:
 * 		24.05.09.04.33
 */

int
sprintint( int lim, unsigned char * buff, int num)
{
	unsigned char intbuf[10];
	int i = 0;
	int j= 0;
	if(num < 0)
	{
		intbuf[i] = '-';
		++i;
		num*=-1;
	}
	while( num != 0)
	{
		intbuf[i] = num % 10 + 48;
		num = num/10;
		++i;
	}
	if( i <= lim)
	{
		for( j = 0; j < i; ++j)
		{
			buff[j] = intbuf[i-j -1];
		}
	}
	return j;
}
/*
 * Function: strcopy
 * Description:
 * 		 equals als ANSI strcopy
 *
 */

int
strcopy( unsigned char *source, unsigned char *dest)
{
	int i = 0;
	while( (source[i] != '\0'))
	{
		dest[i] = source[i];
		++i;
	}
	return i;
}

/*
 * Function: sprintstr
 * Description:
 * 		equals als strcopy but with a limit, in order 
 * 		not to copy out of the limits of buff.
 * 		(FUNCTION USED BY mysprintf)
 */ 
int
sprintstr(int lim, unsigned char *buff, unsigned char *source)
{
	int i = 0;
	if( ((strlen((char *)source)) <= lim))
		i =strcopy(source,buff);
	return i;
}
