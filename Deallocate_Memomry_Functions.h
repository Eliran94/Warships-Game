/**
* This header file declare the functions that free the memory was allocated
*/

#ifndef DEALLOCATE_MEMORY_FUNCTIONS_H
#define DEALLOCATE_MEMORY_FUNCTIONS_H

#include "Board_Struct.h"
#include "Player_Struct.h"

/**
* This function free the memory was allocated for the red board of each player
*/
void free_red_board(struct _board* b);

/**
* This function free the memory of each row of the int matrix, which represent the board,
* until row number i. It also free the board matrix itself
*/
void free_prev_rows(struct _board* b, int i);

/**
* This function free the memory of each ship of the ships' array, and its points' array,
* until ship number i. It also free the ships' array itself
*/
void free_prev_ships(struct _board* b, int i);

/**
* This function free the memory was allocated for the blue board of each player
*/
void free_blue_board(struct _board* b);

/**
* This function free the memory was allocated for a player
*/
void free_player(struct _player* player);


#endif