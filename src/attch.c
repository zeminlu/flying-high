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
attachTable( S_TABLE **table) {

    /*
     * Attach the shared memory :
     */
	key_t a;
    shmp = shmat(shmid, &a);
    if ( shmp == (char *)(-1) ) {
        puts("Error al crear la shm");
		return;
    }

    (*table) = (S_TABLE *)shmp;
}