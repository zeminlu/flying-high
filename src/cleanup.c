/**
 *	\file cleanup.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "bttlship.h"

/*
 * Cleanup procedure called by atexit processing:
 */
void
cleanup(key_t shmid, key_t semid) {

    if ( shmid != -1 )
        if ( shmdt(shmid) == -1 )
	
    if ( semid != -1 )
		sem_free(semid);
}

/* End cleanup.c */
