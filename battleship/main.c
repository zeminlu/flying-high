/* main.c */


#include "bttlship.h"

int shmid = -1;             /* Shared Memory IPC ID */
int semid = -1;             /* Table locking semaphore */
char *shmp = 0;             /* Pointer to shared memory */
int us = 0;                 /* 0=starter / 1=challenger */
int them = 0;               /* 1=challenger / 0=starter */
int flg_game_over = 0;      /* != 0 => game over */

struct S_TABLE *table = 0;  /* Shared Memory Table */

int
main(int argc,char **argv) {
    union semun semarg;         /* for semctl() */
    ushort seminit[] = { 1, 0 };/* Initial sem values */
    pid_t p1, p2;               /* PID for player 1 & 2 */
    char buf[256];              /* For fgets() */
    int x, y, z;                /* move x,y and status z */

    srand(getpid());            /* Init random no.s */

    if ( argc == 1 ) {          /* No args? */
        /*
         * Create a new game :
         */
        /*atexit(cleanup);*/		/* Carga la funcion para ejecutar cuando terminar el proceso */

        /*
         * Create Shared Memory
         */
        shmid = shmget(IPC_PRIVATE,sizeof *table);
        if ( shmid == -1 ) {
            return 13;
        }

        attachTable();          /* Attach new table */

        /*
         * Create a binary semaphore set :
         */
        /*semid = semget(IPC_PRIVATE,2,0666);*/
		semid = sem_get();
        if ( semid == -1 ) {
            return 13;
        }
        
        /*
         * Initialize semaphores:
         */
        semarg.array = seminit;
        if ( semctl(semid,0,SETALL,semarg) == -1 ) {
            puts ("semctl(SETALL)");
            return 13;
        }

        /*
         * Initialize & Generate Battle in shared
         * memory table :
         */
        LOCK;                       /* Wait on semaphore */
        table->semid = semid;       /* Make IPC ID public */
        table->player[0].pid = getpid();
        table->player[1].pid = 0;   /* No opponent yet */
        genBattle();                /* Generate battle */

        us = 0;                     /* We're player [0]  */
        them = 1;                   /* They're player[1] */

        /*
         * Wait for challenger to notify us :
         */
        showBattle();
        UNLOCK;                     /* Notify semaphore */

        /*printf("\n*** GAME # %d ***\n",shmid);*/
		putchar('\n');
		puts("*** GAME #");
		puti(shmid);
		puts(" ***");
		putchar('\n');
        puts("Waiting for opponent...");

        WAIT2;
		putchar('\n');
        puts("THE BATTLE BEGINS!");
		putchar('\n');
    } else {
        /*
         * Opponent is joining a game :
         */
        us = 1;                 /* We're player[1] */
        them = 0;               /* They're player[0] */

        shmid = atoi(argv[1]);  /* Simple int conversion */
        attachTable();          /* Attach existing shm */

        /* No lock is required for this fetch: */
        semid = table->semid;   /* Locking semaphore ID */

        LOCK;                   /* Wait on semaphore */
        p1 = table->player[0].pid;
        p2 = table->player[1].pid;
        if ( p2 == 0 )          /* No opponent yet? */
            p2 = table->player[1].pid = getpid();
        UNLOCK;                 /* Notify semaphore */

        if ( p2 != getpid() ) {
			puts("Sorry: PID ");
			puti(p1);
			puts(" are already playing.");
			putchar('\n');
            return 1;
        }

		puts("You are battling PID ");
		puti(p1);
		putchar('\n');

        LOCK;
        showBattle();
        UNLOCK;

        puts("Press RETURN: ");
        fgets(buf,sizeof buf,stdin);

        NOTIFY2;                /* Notify player1 */
    }

    /*
     * The battle rages :
     */
    while ( !flg_game_over ) {
        if ( (z = getInput(&x,&y)) == INP_NONE ) {
            LOCK;               /* Lock semaphore */
            showBattle();       /* Display battle scene */
            UNLOCK;             /* Unlock semaphore */
            /*fflush(stdout);*/
			refreshStdout();
			refreshScreen();
        } else {  /* INP_YX */
            LOCK;               /* Lock semaphore */
            bomb(x,y);          /* Bomb this location */
            showBattle();       /* Now show results */
            UNLOCK;             /* Unlock semaphore */
        }
    }

    /*
     * Battle is over:
     */
    LOCK;
    showBattle();
    UNLOCK;
    puts("GAME OVER!");

    if ( !us )      /* Player1? */
        WAIT2;      /* Yes, Wait for opponent to finish */
    else
        NOTIFY2;    /* Notify player 1 that we're done */

    return 0; /* Cleanup removes IPC resource for player1 */
}

