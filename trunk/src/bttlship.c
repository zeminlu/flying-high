/**
 *	\file bttlship.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "bttlship.h"

int shmid = -1;             /* Shared Memory IPC ID */
int semid = -1;             /* Table locking semaphore */
int waitSemid = -1;
char *shmp = 0;             /* Pointer to shared memory */
int us[2] = {0,0};                 /* 0=starter / 1=challenger */
int them[2] = {0,0};               /* 1=challenger / 0=starter */
int flg_game_over = 0;      /* != 0 => game over */

struct S_TABLE *table[2] = {0,0};  /* Shared Memory Table */

int
battleship() {
 
   pid_t p1, p2;               /* PID for player 1 & 2 */
   char auxChar = '0';              /* For fgets() */
   int x, y, z;                /* move x,y and status z */

    srand(getpid());            /* Init random no.s */
	
	puts("Bienvenido a Flying-High-BattleShip\n\n");
	puts("Debo hostear una partida?\n\t");

	while (auxChar != 's' && auxChar != 'n') {
		puts("Solo conteste s o n\n");
		auxChar = getchar();
		if(auxChar == EOF){
			waitTty(getTty(getpid()));
			continue;
		}
	}

    if ( auxChar == 's' ) {          /* No args? */
      
        /*
         * Create Shared Memory
         */
        shmid = shmget(IPC_PRIVATE, sizeof **table);
		if ( shmid == -1 ) {
            return 13;
        }

        attachTable();          /* Attach new table */
		
		semid = sem_get(BLOCK);
		if ( semid == -1 ) {
            return 13;
        }

		waitSemid = sem_get(WAIT);
		if ( waitSemid == -1 ) {
            return 13;
        }

		table[0]->semid = semid;       /* Make IPC ID public */
		table[0]->waitSemid = waitSemid;
		table[1]->semid = semid;       /* Make IPC ID public */
		table[1]->waitSemid = waitSemid;
        
        /*
         * Initialize & Generate Battle in shared
         * memory table :
         */
        LOCK;                       /* Wait on semaphore */
		table[0]->player[0].pid = getpid();
        table[0]->player[1].pid = 0;   /* No opponent yet */
		table[1]->player[0].pid = getpid();
        table[1]->player[1].pid = 0;   /* No opponent yet */
        genBattle(0);                /* Generate battle */

        us[0] = 0;                     /* We're player [0]  */
        them[0] = 1;                   /* They're player[1] */

        /*
         * Wait for challenger to notify us :
         */
        showBattle(0);
        UNLOCK;                     /* Notify semaphore */

		puts("\n*** GAME #");
		puti(shmid);
		puts(" ***\nWaiting for opponent...\n");

        WAIT2;
        puts("\nTHE BATTLE BEGINS!\n");
    } else {
        /*
         * Opponent is joining a game :
         */
        us[1] = 1;                 /* We're player[1] */
        them[1] = 0;               /* They're player[0] */
		auxChar = 'A';
		
		puts("\nPor favor ingrese el id que el proceso que hostea le ha proporcionado:\n");

		while (auxChar < '0' || auxChar > '9') {
			puts("Ingrese un id entero por favor\n");
			auxChar = getchar();
			if(auxChar == EOF){
				waitTty(getTty(getpid()));
				continue;
			}
		}
			
		shmid = auxChar - '0';  /* Simple int conversion */
		attachTable();          /* Attach existing shm */

        /* No lock is required for this fetch: */
        semid = table[0]->semid;   /* Locking semaphore ID */
		waitSemid = table[0]->waitSemid;

        LOCK;                   /* Wait on semaphore */
		p1 = table[0]->player[0].pid;
        p2 = table[0]->player[1].pid;
        if ( p2 == 0 )          /* No opponent yet? */
            p2 = table[0]->player[1].pid = getpid();
        UNLOCK;                 /* Notify semaphore */
		
		if ( p2 != getpid() ) {
			puts("Sorry: PID ");
			puti(p1);
			puts(" is already playing.");
			putchar('\n');
            return 1;
        }

		puts("You are battling PID ");
		puti(p1);
		putchar('\n');

        LOCK;
        showBattle(1);
        UNLOCK;

        puts("Press any key:");
		while(getchar() == EOF){
			waitTty(getTty(getpid()));
		}
		puts("\n\n");
		
        NOTIFY2;                /* Notify player1 */
    }

    /*
     * The battle rages :
     */
    while ( !flg_game_over ) {
        if ( (z = getInput(&x,&y)) == INP_NONE ) {
            LOCK;               /* Lock semaphore */
            if( sysGetTTYFocusedProcess(getFocusedTTY()) == table[0]->player[0].pid )
				showBattle(0);       /* Display battle scene */
			else
				showBattle(1);
            UNLOCK;             /* Unlock semaphore */
            /*fflush(stdout);*/
			refreshStdout();
			refreshScreen();
        } else {  /* INP_YX */
            LOCK;               /* Lock semaphore */
            if( sysGetTTYFocusedProcess(getFocusedTTY()) == table[0]->player[0].pid ){
				bomb(x,y, 0);          /* Bomb this location */
			    showBattle(0);       /* Now show results */
			}else{
				bomb(x,y,1);          /* Bomb this location */
			    showBattle(1);       /* Now show results */
			}
            UNLOCK;             /* Unlock semaphore */
        }
    }

    /*
     * Battle is over:
     */
    LOCK;
    showBattle(0);
    UNLOCK;
    puts("GAME OVER!\n");

    if ( !us[0] )      /* Player1? */
        WAIT2;      /* Yes, Wait for opponent to finish */
    else
        NOTIFY2;    /* Notify player 1 that we're done */

    return 0; /* Cleanup removes IPC resource for player1 */
}