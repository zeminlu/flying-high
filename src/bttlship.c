/**
 *	\file bttlship.c
 *
 *		\brief Battleship module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "bttlship.h"

int battleship() {
 
   	pid_t p1, p2;               /* PID for player 1 & 2 */
   	char auxChar = '0';              /* For fgets() */
   	int x, y, z;                /* move x,y and status z */
	S_TABLE *table = NULL;
	int us = 0;                 /* 0=starter / 1=challenger */
	int them = 0;               /* 1=challenger / 0=starter */
	int shmid = -1;             /* Shared Memory IPC ID */
	int semid = -1;             /* Table locking semaphore */
	int waitSemid = -1;
	char *shmp = 0;             /* Pointer to shared memory */
	int flg_game_over = 0;      /* != 0 => game over */
	pid_t pid;
	
	pid = getpid();
	
    srand(pid);            /* Init random no.s */
	
	puts("Bienvenido a Flying-High-BattleShip\n\n");
	puts("Debo hostear una partida?\n\t");

	while (auxChar != 's' && auxChar != 'n') {
		puts("Solo conteste s o n\n");
		auxChar = getchar();
		if(auxChar == EOF){
			waitTty(getTty(pid));
			continue;
		}
	}

    if ( auxChar == 's' ) {
        /*
         * Create Shared Memory
         */
        shmid = shmget(IPC_PRIVATE, sizeof( *table));
		if ( shmid == -1 ) {
            return 13;
        }

        attachTable(&table, shmid, &shmp);          /* Attach new table */
		
		semid = sem_get(BLOCK);
		if ( semid == -1 ) {
            return 13;
        }

		waitSemid = sem_get(WAIT);
		if ( waitSemid == -1 ) {
            return 13;
        }

		table->semid = semid;       /* Make IPC ID public */
		table->waitSemid = waitSemid;
        
        /*
         * Initialize & Generate Battle in shared
         * memory table :
         */
        LOCK;                       /* Wait on semaphore */
		table->player[0].pid = pid;
        table->player[1].pid = MAX_PROCESS;   /* No opponent yet */
        genBattle(&table);                /* Generate battle */

        us = 0;                     /* We're player [0]  */
        them = 1;                   /* They're player[1] */

        /*
         * Wait for challenger to notify us :
         */
        showBattle(&table, us, them, &flg_game_over);
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
        us = 1;                 /* We're player[1] */
        them = 0;               /* They're player[0] */
		auxChar = 'A';
		
		puts("\nPor favor ingrese el id que el proceso que hostea le ha proporcionado:\n");

		while (auxChar < '0' || auxChar > '9') {
			puts("Ingrese un id entero por favor\n");
			auxChar = getchar();
			if(auxChar == EOF){
				waitTty(getTty(pid));
				continue;
			}
		}
			
		shmid = auxChar - '0';  /* Simple int conversion */
		attachTable(&table, shmid, &shmp);    /* Attach existing shm */

        /* No lock is required for this fetch: */
        semid = table->semid;   /* Locking semaphore ID */
		waitSemid = table->waitSemid;

        LOCK;                   /* Wait on semaphore */
		p1 = table->player[0].pid;
        p2 = table->player[1].pid;
        if ( p2 == MAX_PROCESS )          /* No opponent yet? */
            p2 = table->player[1].pid = pid;
        UNLOCK;                 /* Notify semaphore */
		
		if ( p2 != pid ) {
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
        showBattle(&table, us, them, &flg_game_over);
        UNLOCK;

        puts("Press any key:");
		while(getchar() == EOF){
			waitTty(getTty(pid));
		}
		puts("\n\n");
		
        NOTIFY2;                /* Notify player1 */
    }

    /*
     * The battle rages :
     */
    while ( !flg_game_over ) {
        if ( (z = getInput(&x,&y, &table)) == INP_NONE ) {
            LOCK;               /* Lock semaphore */
            showBattle(&table, us, them, &flg_game_over);
            UNLOCK;             /* Unlock semaphore */
			refreshStdout();
			refreshScreen();
        } else {  /* INP_YX */
            LOCK;               /* Lock semaphore */
			bomb(x,y, &table);          /* Bomb this location */
			showBattle(&table, us, them, &flg_game_over);       /* Now show results */
            UNLOCK;             /* Unlock semaphore */
        }
    }

    /*
     * Battle is over:
     */
    LOCK;
    showBattle(&table, us, them, &flg_game_over);
    UNLOCK;
    puts("GAME OVER!\n");

    if ( !us )      /* Player1? */
        WAIT2;      /* Yes, Wait for opponent to finish */
    else
        NOTIFY2;    /* Notify player 1 that we're done */

    return 0; /* Cleanup removes IPC resource for player1 */
}

/*
 * Cleanup procedure called by atexit processing:
 */
void cleanup(key_t shmid, key_t semid) {

    if ( shmid != -1 )
        if ( shmdt(shmid) == -1 )
	
    if ( semid != -1 )
		sem_free(semid);
}

void attachTable( S_TABLE **table, key_t shmid, char **shmp) {

    /*
     * Attach the shared memory :
     */
	key_t a;
    *shmp = shmat(shmid, &a);
    if ( *shmp == (char *)(-1) ) {
        puts("Error al crear la shm");
		return;
    }

    (*table) = *((S_TABLE **)shmp);
}

void recount(S_TABLE **tableS, int *flg_game_over) {
    int b, f, x, y, px;
	S_TABLE *table = *tableS;
	
    table->player[0].bsites = table->player[1].bsites = 0;
    for ( y=0; y<N_Y; ++y ) {
        for ( x=0; x<N_X; ++x ) {
            f = table->flg[x][y];
            if ( !(f & (FLG_P1|FLG_P2)) )
                continue;   /* Sea zone */
            px = f & FLG_P1 ? 0 : 1;
            if ( !(f & FLG_BOMBD) )
                ++table->player[px].bsites;
            b = table->sea[x][y];
        }
    }
    if ( !table->player[0].bsites
    ||   !table->player[1].bsites )
        *flg_game_over = 1;
}

void bomb(int x,int y, S_TABLE ** tableS ) {
	S_TABLE *table = *tableS;
	if ( table->flg[x][y] & (FLG_P1|FLG_P2) ){
		table->flg[x][y] |= FLG_BOMBD;
	}
    else{
	    table->flg[x][y] |= FLG_SPLSH;
	}
}

int getInput(int *px,int *py, S_TABLE **table) {
    char cr = 0, cc = 0;
    char cx, cy, mx, my;
	pid_t pid;
	
	pid = getpid();
	
    mx = 'A' + N_X - 1;
    my = 'A' + N_Y - 1;

    puts("Next bomb: ");
	
	while ( cr < 'A' || cr > mx) {
		cr = getchar();
		if(cr == EOF){
			waitTty(getTty(pid));
			continue;
		}
	}
	putchar(cr);
	putchar(' ');
	while ( cc < 'A' || cc > my) {
		cc = getchar();
		if(cc == EOF){
			waitTty(getTty(pid));
			continue;
		}
	}
	putchar(cc);
	putchar('\n');

    cx = cr;
    cy = cc;
    if ( cx < 'A' || cx > mx
    ||   cy < 'A' || cy > my )
        return INP_NONE; /* Bad -- show battle */

    *px = cx - 'A';
    *py = cy - 'A';
    return INP_YX;      /* Bombs away! */
}

int draw_hz(int sx,int sy,int z,int who, S_TABLE **tableS) {
    int x, ex;
    char ch = who ? '=' : '-';
    char f = who ? FLG_P1 : FLG_P2;
	S_TABLE *table = *tableS;

    if ( sx + z > N_X ) {
        if ( (ex = sx - z) < 0 )
            return 0;       /* Can't */
        for ( x=sx; x>ex; --x )
            if ( table->sea[x][sy] != '.'
            || ( sy+1 < N_Y && table->sea[x][sy+1] != '.' )
            || ( sy-1 >= 0  && table->sea[x][sy-1] != '.' ) )
                return 0;   /* Can't */
        for ( x=sx; x>ex+1; --x ) {
            table->sea[x][sy] = ch;
            table->flg[x][sy] = f;
	}    
        table->sea[ex+1][sy] = '<';
        table->flg[ex+1][sy] = f;
    } else  {
        ex = sx + z;
        for ( x=sx; x<ex; ++x )
            if ( table->sea[x][sy] != '.'
            || ( sy+1 < N_Y && table->sea[x][sy+1] != '.' )
            || ( sy-1 >= 0  && table->sea[x][sy-1] != '.' ) )
                return 0;   /* Can't */
        for ( x=sx; x<ex-1; ++x ) {
            table->sea[x][sy] = ch;
            table->flg[x][sy] = f;
        }
        table->sea[ex-1][sy] = '>';
        table->flg[ex-1][sy] = f;
    }
    return 1;   /* Drawn */
}

int draw_vt(int sx,int sy,int z,int who, S_TABLE **tableS) {
    int y, ey;
    char ch = who ? '!' : '|';
    char f = who ? FLG_P1 : FLG_P2;
	S_TABLE *table = *tableS;

    if ( sy + z > N_Y ) {
        if ( (ey = sy - z) < 0 )
            return 0;       /* Can't */
        for ( y=sy; y>ey; --y )
            if ( table->sea[sx][y] != '.'
            || ( sx+1 < N_X && table->sea[sx+1][y] != '.' )
            || ( sx-1 >= 0  && table->sea[sx-1][y] != '.' ) )
                return 0;   /* Can't */
        for ( y=sy; y>ey+1; --y ) {
            table->sea[sx][y] = ch;
            table->flg[sx][y] = f;
        }
        table->sea[sx][ey+1] = '^';
        table->flg[sx][ey+1] = f;
    } else {
        ey = sy + z;
        for ( y=sy; y<ey; ++y )
            if ( table->sea[sx][y] != '.'
            || ( sx+1 < N_X && table->sea[sx+1][y] != '.' )
            || ( sx-1 >= 0  && table->sea[sx-1][y] != '.' ) )
                return 0;   /* Can't */
        for ( y=sy; y<ey-1; ++y ) {
            table->sea[sx][y] = ch;
            table->flg[sx][y] = f;
        }
        table->sea[sx][ey-1] = 'V';
        table->flg[sx][ey-1] = f;
    }        
    return 1;   /* Drawn */
}

void genBattle(S_TABLE **tableS) {
    int x, y, z, dir;
    int count = 0;
    int drawn = 0;
	S_TABLE *table = *tableS;
    
    memset(table->sea,'.',sizeof table->sea);
    memset(table->flg,0,sizeof table->flg);

    do  {
        if ( count > 10000 ) {
            srand(timeRand());       /* Re-seed the generator */
            count = 0;
        } else
            ++count;        /* Increment the safety cnt */

        z = rand() % N_Z + 2; /* Length of ship */
        dir = rand() & 1;
        x = rand() % N_X;
        y = rand() % N_Y;
        if ( dir ) {
            if ( draw_hz(x,y,z,drawn&1, tableS)
            ||   draw_vt(x,y,z,drawn&1, tableS) )
                ++drawn;
        } else {
            if ( draw_vt(x,y,z,drawn&1, tableS)
            ||   draw_hz(x,y,z,drawn&1, tableS) )
                ++drawn;
        }
    } while ( drawn < N_SHIPS );
}

void showRow(void) {
    int x;

    fputs("  ",stdout);
    for ( x=0; x<N_X; ++x ) {
        fputc(' ',stdout);
        fputc('A'+x,stdout);
    }
    fputc('\n',stdout);
}

void showBattle( S_TABLE **tableS, int us, int them, int *flg_game_over) {
    int x, y;
    char f = !us
        ? (FLG_SEEN1|FLG_P1)
        : (FLG_SEEN0|FLG_P2);
	S_TABLE *table = *tableS;
	
    showRow();

    for ( y=0; y<N_Y; ++y ) {
        fputc('A'+y,stdout);
        fputc('|',stdout);
        for ( x=0; x<N_X; ++x ) {
            fputc(' ',stdout);
            if ( table->flg[x][y] & FLG_BOMBD ) {
                if ( table->flg[x][y] & FLG_P1 )
                    fputc('@',stdout);
                else
                    fputc('#',stdout);
            } else if ( table->flg[x][y] & f )
                fputc(table->sea[x][y],stdout);
            else if ( table->flg[x][y] & FLG_SPLSH )
                fputc('*',stdout);
            else
                fputc('.',stdout);
        }
        puts(" |\n");
    }

    showRow();
    recount(tableS, flg_game_over);
	puts("ENEMY HAS ");
	puti(table->player[them].bsites);
	puts(" BOMB SITES LEFT");
	putchar('\n');
	puts("YOU HAVE  ");
	puti(table->player[us].bsites);
	puts(" BOMB SITES LEFT");
	putchar('\n');
}

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
