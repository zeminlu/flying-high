/* attch.c */

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

    table = (struct S_TABLE *)shmp;
}

/* End attch.c */
