/* cleanup.c */

#include "bttlship.h"

/*
 * Cleanup procedure called by atexit processing:
 */
void
cleanup(void) {
    union semun semarg;

    /*
     * Detach shared memory if it is attached:
     */
    /*if ( shmp != 0 && shmp != (char *)(-1) )
        if ( shmdt(shmp) == -1 ){
			puts("Error al borrar Shm si esta atachada");
		}
*/
    /*
     * Destroy shared memory:
     */
    if ( shmid != -1 )
        if ( shmdt(shmid) == -1 )
			puts("Error al borrar Shm");
    /*
     * Destroy semaphore:
     */
    if ( semid != -1 )
        if ( sem_free(semid) == -1 )
            puts("Error al borrar semaforos");
}

/* End cleanup.c */
