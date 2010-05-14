/*
 *	rand.h
 */

#include "rand.h"

static unsigned int seed;

unsigned int timeRand(){
	return (unsigned int)_sys_uptime();
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
