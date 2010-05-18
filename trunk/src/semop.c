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
lockTable(int semx,int block, S_TABLE **tableS) {
	
	S_TABLE *table = *tableS;
	
	if( !semx ){
		if( block ){
			sem_wait(table->semid);
		}else{
			sem_signal(table->semid);
		}
	}else{
		if( block ){
			sem_wait(table->waitSemid);
		}else{
			sem_signal(table->waitSemid);
		}
	}
}

/* End semop.c */
