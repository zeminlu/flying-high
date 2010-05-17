/**
 *	\file attch.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "bttlship.h"

void
attachTable(void) {

    /*
     * Attach the shared memory :
     */
	key_t a;
    shmp = shmat(shmid, &a);
    if ( shmp == (char *)(-1) ) {
        puts("Error al crear la shm");
		return;
    }

    table[0] = (struct S_TABLE *)shmp;
	table[1] = (struct S_TABLE *)shmp;
}