/**
 *	\file game.c
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "bttlship.h"

void
recount(int p) {
    int b, f, x, y, px;

    table[p]->player[0].bsites = table[p]->player[1].bsites = 0;
    for ( y=0; y<N_Y; ++y ) {
        for ( x=0; x<N_X; ++x ) {
            f = table[p]->flg[x][y];
            if ( !(f & (FLG_P1|FLG_P2)) )
                continue;   /* Sea zone */
            px = f & FLG_P1 ? 0 : 1;
            if ( !(f & FLG_BOMBD) )
                ++table[p]->player[px].bsites;
            b = table[p]->sea[x][y];
        }
    }
    if ( !table[p]->player[0].bsites
    ||   !table[p]->player[1].bsites )
        flg_game_over = 1;
}

void
bomb(int x,int y, int player) {
    if ( table[player]->flg[x][y] & (FLG_P1|FLG_P2) )
        table[player]->flg[x][y] |= FLG_BOMBD;
    else
        table[player]->flg[x][y] |= FLG_SPLSH;
}

int
getInput(int *px,int *py) {
    char cr = 0, cc = 0;
    char cx, cy, mx, my;

    mx = 'A' + N_X - 1;
    my = 'A' + N_Y - 1;

    puts("Next bomb: ");
	
	while ( cr < 'A' || cr > mx) {
		cr = getchar();
		if(cr == EOF){
			waitTty(getTty(getpid()));
			continue;
		}
	}
	putchar(cr);
	putchar(' ');
	while ( cc < 'A' || cc > my) {
		cc = getchar();
		if(cc == EOF){
			waitTty(getTty(getpid()));
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

int
draw_hz(int sx,int sy,int z,int who, int player) {
    int x, ex;
    char ch = who ? '=' : '-';
    char f = who ? FLG_P1 : FLG_P2;

    if ( sx + z > N_X ) {
        if ( (ex = sx - z) < 0 )
            return 0;       /* Can't */
        for ( x=sx; x>ex; --x )
            if ( table[player]->sea[x][sy] != '.'
            || ( sy+1 < N_Y && table[player]->sea[x][sy+1] != '.' )
            || ( sy-1 >= 0  && table[player]->sea[x][sy-1] != '.' ) )
                return 0;   /* Can't */
        for ( x=sx; x>ex+1; --x ) {
            table[player]->sea[x][sy] = ch;
            table[player]->flg[x][sy] = f;
	}    
        table[player]->sea[ex+1][sy] = '<';
        table[player]->flg[ex+1][sy] = f;
    } else  {
        ex = sx + z;
        for ( x=sx; x<ex; ++x )
            if ( table[player]->sea[x][sy] != '.'
            || ( sy+1 < N_Y && table[player]->sea[x][sy+1] != '.' )
            || ( sy-1 >= 0  && table[player]->sea[x][sy-1] != '.' ) )
                return 0;   /* Can't */
        for ( x=sx; x<ex-1; ++x ) {
            table[player]->sea[x][sy] = ch;
            table[player]->flg[x][sy] = f;
        }
        table[player]->sea[ex-1][sy] = '>';
        table[player]->flg[ex-1][sy] = f;
    }
    return 1;   /* Drawn */
}

int
draw_vt(int sx,int sy,int z,int who, int player) {
    int y, ey;
    char ch = who ? '!' : '|';
    char f = who ? FLG_P1 : FLG_P2;

    if ( sy + z > N_Y ) {
        if ( (ey = sy - z) < 0 )
            return 0;       /* Can't */
        for ( y=sy; y>ey; --y )
            if ( table[player]->sea[sx][y] != '.'
            || ( sx+1 < N_X && table[player]->sea[sx+1][y] != '.' )
            || ( sx-1 >= 0  && table[player]->sea[sx-1][y] != '.' ) )
                return 0;   /* Can't */
        for ( y=sy; y>ey+1; --y ) {
            table[player]->sea[sx][y] = ch;
            table[player]->flg[sx][y] = f;
        }
        table[player]->sea[sx][ey+1] = '^';
        table[player]->flg[sx][ey+1] = f;
    } else {
        ey = sy + z;
        for ( y=sy; y<ey; ++y )
            if ( table[player]->sea[sx][y] != '.'
            || ( sx+1 < N_X && table[player]->sea[sx+1][y] != '.' )
            || ( sx-1 >= 0  && table[player]->sea[sx-1][y] != '.' ) )
                return 0;   /* Can't */
        for ( y=sy; y<ey-1; ++y ) {
            table[player]->sea[sx][y] = ch;
            table[player]->flg[sx][y] = f;
        }
        table[player]->sea[sx][ey-1] = 'V';
        table[player]->flg[sx][ey-1] = f;
    }        
    return 1;   /* Drawn */
}

void
genBattle(int player) {
    int x, y, z, dir;
    int count = 0;
    int drawn = 0;
    /*time_t t;*/
    
    memset(table[1]->sea,'.',sizeof table[player]->sea);
    memset(table[1]->flg,0,sizeof table[player]->flg);
    memset(table[0]->sea,'.',sizeof table[player]->sea);
    memset(table[0]->flg,0,sizeof table[player]->flg);

    do  {
        if ( count > 10000 ) {
            /*time(&t);*/
            srand(timeRand());       /* Re-seed the generator */
            count = 0;
        } else
            ++count;        /* Increment the safety cnt */

        z = rand() % N_Z + 2; /* Length of ship */
        dir = rand() & 1;
        x = rand() % N_X;
        y = rand() % N_Y;
        if ( dir ) {
            if ( draw_hz(x,y,z,drawn&1, 0)
            ||   draw_vt(x,y,z,drawn&1, 0) )
                ++drawn;
        } else {
            if ( draw_vt(x,y,z,drawn&1, 1)
            ||   draw_hz(x,y,z,drawn&1, 1) )
                ++drawn;
        }
    } while ( drawn < N_SHIPS );
}

void
showRow(void) {
    int x;

    fputs("  ",stdout);
    for ( x=0; x<N_X; ++x ) {
        fputc(' ',stdout);
        fputc('A'+x,stdout);
    }
    fputc('\n',stdout);
}

void
showBattle( int p ) {
    int x, y;
    char f = !us[p]
        ? (FLG_SEEN1|FLG_P1)
        : (FLG_SEEN0|FLG_P2);

    showRow();

    for ( y=0; y<N_Y; ++y ) {
        fputc('A'+y,stdout);
        fputc('|',stdout);
        for ( x=0; x<N_X; ++x ) {
            fputc(' ',stdout);
            if ( table[p]->flg[x][y] & FLG_BOMBD ) {
                if ( table[p]->flg[x][y] & FLG_P1 )
                    fputc('@',stdout);
                else
                    fputc('#',stdout);
            } else if ( table[p]->flg[x][y] & f )
                fputc(table[p]->sea[x][y],stdout);
            else if ( table[p]->flg[x][y] & FLG_SPLSH )
                fputc('*',stdout);
            else
                fputc('.',stdout);
        }
        puts(" |\n");
    }

    showRow();
    recount(p);
	puts("ENEMY HAS ");
	puti(table[p]->player[them[p]].bsites);
	puts(" BOMB SITES LEFT");
	putchar('\n');
	puts("YOU HAVE  ");
	puti(table[p]->player[us[p]].bsites);
	puts(" BOMB SITES LEFT");
	putchar('\n');
}

/* End game.c */
