/**
 *	\file rand.c
 *
 *		\brief Random number generation module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#include "rand.h"

static unsigned int seed;

unsigned int timeRand(){
	return (unsigned int)upTime();
}

int rand(){
	
	static int a = 11035152;
	static int b = 12345;
	
	seed = ( a * seed + b ) % RAND_MAX;
	return seed;
}

void srand( unsigned int seedP ){
	seed = seedP;
}
