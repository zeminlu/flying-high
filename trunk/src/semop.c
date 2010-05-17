/**
 *	\file semop.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */
#include "bttlship.h"

/*
 * Peform semaphore wait/notifies:
 * ARGUMENTS:
 *  semx    0 : table lock semaphore
 *          1 : opponent notify semaphore
 *  bLock   0 : perform notify
 *          1 : perform wait
 */
void
lockTable(int semx,int block) {
	
	if( !semx ){
		if( block ){
			sem_wait(table[0]->semid);
		}else{
			sem_signal(table[0]->semid);
		}
	}else{
		if( block ){
			sem_wait(table[0]->waitSemid);
		}else{
			sem_signal(table[0]->waitSemid);
		}
	}
}

/* End semop.c */
