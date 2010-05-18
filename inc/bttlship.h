/**
 *	\file bttlship.h
 *
 *		\brief The header file of the battleship game.
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
#include "shMemory.h"
#include "semaphore.h"
#include "rand.h"
#include "ttys.h"

#define N_X         10      /* X dimension */
#define N_Y         10      /* Y dimension */
#define N_Z         5       /* Max length of ship */
#define N_SHIPS     6       /* Max number of ships */

#define INP_NONE    0       /* No input not recognizable */
#define INP_YX      1       /* YX was given */

#define LOCK        lockTable(0,1, &table)  /* Lock Table */
#define UNLOCK      lockTable(0,0, &table)  /* Unlock Table */

#define WAIT2       lockTable(1,1, &table)  /* Wait for Notify */
#define NOTIFY2     lockTable(1,0, &table)  /* Notify player 1 */

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
typedef struct S_TABLE {
    int     semid;          /* Locking sem IPC ID */
	int		waitSemid;
    struct  {
      pid_t pid;            /* Process ID of player */
      int   bsites;         /* Sites left for bombing */
    }       player[2];
    char    sea[N_X][N_Y];  /* Matrix of sea locations */
    char    flg[N_X][N_Y];  /* Flags */
}S_TABLE;

/**
 * \fn void cleanup(key_t shmid, key_t semid)
 *
 * 		\brief Destroys the used shared memory and semaphores
 *
 *		\param shmid The id of the shared memory segment.
 *		\param semid The id of the semaphore.
 *
 */
extern void cleanup(key_t shmid, key_t semid);

/**
 * \fn void attachTable(S_TABLE **table, key_t shmid, char **shmp);
 *
 * 		\brief It attaches itself to the already
 *				created memory.
 *
 *		\param table The adress of the table.
 *		\param shmid The shared memory segment id.
 *		\param shmp The adress where to store the shared memory segment pointer.
 *
 */
extern void attachTable(S_TABLE **table, key_t shmid, char **shmp);

/**
 * \fn void lockTable(int semx,int block, S_TABLE **tableS)
 *
 * 		\brief It decides if it has to lock the table
 *				or wait for the opponent. Peform semaphore wait/notifies:
 * 				ARGUMENTS:
 *  				semx    0 : table lock semaphore
 *          				1 : opponent notify semaphore
 *  				block   0 : perform notify
 *          				1 : perform wait
 *
 * 		\param semx It is the parameter to decide what have to do with semaphore
 * 		\param block It decides what have to do with the table
 *		\param tableS The adress of the table.
 * 		
 */
extern void lockTable(int semx,int block, S_TABLE **tableS);

/**
 * \fn void recount(S_TABLE **tableS, int *flg_game_over)
 *
 * 		\brief It counts the amount of bombs that the player has left
 *
 *		\param tableS The address of the table.
 *		\param flg_game_over The adress of the game status.
 *
 */
extern void recount(S_TABLE **tableS, int *flg_game_over);

/**
 * \fn void bomb(int x, int y, S_TABLE **tableS)
 *
 * 		\brief It puts the bomb on a specific position of the table
 *
 * 		\param x The row of the table
 * 		\param y The column of the table
 *		\param tableS The address of the table.
 *
 */
extern void bomb(int x, int y, S_TABLE **tableS);

/**
 * \fn int getInput(int *px,int *py, S_TABLE **table)
 *
 * 		\brief It obtains the the row and the column of the table
 *				from STDIN
 *
 * 		\param px The row of the table
 * 		\param py The column of the table
 *		\param table The address of the table.
 *
 */
extern int getInput(int *px,int *py, S_TABLE **table);

/**
 * \fn int draw_hz(int sx,int sy,int z,int who, S_TABLE **tableS)
 *
 * 		\brief It draws the horizontal ship
 *
 * 		\param sx Inner use.
 * 		\param sy Inner use.
 * 		\param z Inner use.
 *		\param who Iner use.
 *		\param tableS The address of the table.
 *
 */
extern int draw_hz(int sx,int sy,int z,int who, S_TABLE **tableS);

/**
 * \fn int draw_vt(int sx,int sy,int z,int who, S_TABLE **tableS)
 *
 * 		\brief It draws the horizontal ship
 *
 * 		\param sx Inner use.
 * 		\param sy Inner use.
 * 		\param z Inner use.
 *		\param who Inner use.
 *		\param tableS The address of the table.
 *
 */
extern int draw_vt(int sx,int sy,int z,int who, S_TABLE **tableS);

/**
 * \fn void genBattle(S_TABLE **tableS)
 *
 * 		\brief It generates the table
 *
 *		\param tableS The address of the table.
 *
 */
extern void genBattle(S_TABLE **tableS);

/**
 * \fn void showRow(void)
 *
 * 		\brief It prints each row of the table
 *
 */
extern void showRow(void);

/**
 * \fn void showBattle(S_TABLE **tableS, int us, int them, int *flg_game_over)
 *
 * 		\brief It prints the table of the battlehsip
 *
 *		\param tableS The address of the table.
 *		\param us Inner use.
 *		\param them Inner use.
 *		\param flg_game_over The address of the game status.
 *
 */
extern void showBattle(S_TABLE **tableS, int us, int them, int *flg_game_over);

/**
 * \fn int battleship()
 *
 * 		\brief It's the main process of the battleship, it generates
 *				the whole table and ships.
 *
 */
int battleship();

#endif
