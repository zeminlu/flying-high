/* semop.c */

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
			puts("semx = 0 && block = 1");
			sem_wait(table->semid);
		}else{
			puts("semx = 0 && block = 0");
			sem_signal(table->semid);
		}
	}else{
		if( block ){
			puts("semx = 1 && block = 1");
			sem_wait(table->waitSemid);
		}else{
			puts("semx = 1 && block = 0");
			sem_signal(table->waitSemid);
		}
	}
}

/* End semop.c */
