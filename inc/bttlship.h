/**
 *	\file bttlship.h
 *
 *		\brief Brief.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#ifndef _BTTLSHIP_H_
#define _BTTLSHIP_H_

#include "video_driver.h"
#include "stdio.h"
#include "string.h"
#include "types.h"
#include "process.h"
#include "defs.h"
#include "shMemory.h"
#include "semaphore.h"
#include "rand.h"
#include "ttys.h"

#define N_X         10      /* X dimension */
#define N_Y         10      /* Y dimension */
#define N_Z         5       /* Max length of ship */
#define N_SHIPS     6       /* Max number of ships */

#define INP_NONE    0       /* No input / not recognizable */
#define INP_YX      1       /* YX was given */

#define LOCK        lockTable(0,1)  /* Lock Table */
#define UNLOCK      lockTable(0,0)  /* Unlock Table */

#define WAIT2       lockTable(1,1)  /* Wait for Notify */
#define NOTIFY2     lockTable(1,0)  /* Notify player 1 */

/*
 * Table flags :
 */
#define FLG_P1      001     /* Owned by player 1 */
#define FLG_P2      002     /* Owned by player 2 */
#define FLG_SEEN0   004     /* Seen by player 1 */
#define FLG_SEEN1   010     /* Seen by player 2 */
#define FLG_BOMBD   020     /* Bombed */
#define FLG_SPLSH   040     /* Splash */

/*
 * Shared Memory Table :
 */
struct S_TABLE {
    int     semid;          /* Locking sem IPC ID */
	int		waitSemid;
    struct  {
      pid_t pid;            /* Process ID of player */
      int   bsites;         /* Sites left for bombing */
    }       player[2];
    char    sea[N_X][N_Y];  /* Matrix of sea locations */
    char    flg[N_X][N_Y];  /* Flags */
};

extern struct S_TABLE *table;

extern int shmid;           /* Shared Memory IPC ID */
extern int semid;           /* Table locking semaphore */
extern char *shmp;          /* Pointer to shared memory */
extern int us;              /* 0=starter / 1=challenger */
extern int them;            /* 1=challenger / 0=starter */
extern int flg_game_over;   /* != 0 => game over */

/**
 * \fn void cleanup(void)
 *
 * 		\brief Destroy the shared memory and semaphores
 *
 */

extern void cleanup(void);

/**
 * \fn void attachTable(void);
 *
 * 		\brief It only attach to the memory already
 *				created.
 *
 */

extern void attachTable(void);

/**
 * \fn void lockTable(int semx,int bLock)
 *
 * 		\brief It decidies if it has to lock the table
 *				or wait to the opponent. Peform semaphore wait/notifies:
 * 				ARGUMENTS:
 *  				semx    0 : table lock semaphore
 *          				1 : opponent notify semaphore
 *  				bLock   0 : perform notify
 *          				1 : perform wait
 *
 * 		\param semx It is the parameter to decide what have to do with semaphore
 * 		\param block It decides what have to do with the table
 * 		
 *
 */

extern void lockTable(int semx,int bLock);

/**
 * \fn void recount(void)
 *
 * 		\brief It count the amount of bomb that the player left
 *
 * 		
 *
 */

extern void recount(void);

/**
 * \fn void bomb(int x,int y)
 *
 * 		\brief It put the bombo in a specific position of the table
 *
 * 		\param x The row of the table
 * 		\param y The column of the table
 * 		
 *
 */

extern void bomb(int x,int y);

/**
 * \fn int getInput(int *px,int *py)
 *
 * 		\brief It obtain the the row and the column of the table
 *				from STDIN
 *
 * 		\param px The row of the table
 * 		\param py The column of the table
 * 		
 *
 */

extern int getInput(int *px,int *py);

/**
 * \fn int draw_hz(int sx,int sy,int z,int who)
 *
 * 		\brief It draws the horizontal ship
 *
 * 		\param sx
 * 		\param sy
 * 		\param z
 *		\param who
 *
 */

extern int draw_hz(int sx,int sy,int z,int who);

/**
 * \fn int draw_vt(int sx,int sy,int z,int who)
 *
 * 		\brief It draws the horizontal ship
 *
 * 		\param sx
 * 		\param sy
 * 		\param z
 *		\param who
 *
 */

extern int draw_vt(int sx,int sy,int z,int who);

/**
 * \fn void genBattle(void)
 *
 * 		\brief It generates the table
 */

extern void genBattle(void);

/**
 * \fn void showRow(void)
 *
 * 		\brief It prints each row of the table
 *
 */

extern void showRow(void);

/**
 * \fn void showBattle(void)
 *
 * 		\brief It prints the whole table of the battlehsip
 *
 *
 */

extern void showBattle(void);

/**
 * \fn int battleship()
 *
 * 		\brief It is the main process of the battleship, it generates
 *				the whole table and ships.
 *
 */

int battleship();

#endif
