/**
* This header file declares functions that initiate the program's types,
* and allocate the required memory
*/

#ifndef INITIATE_FUNCTIONS_H
#define INITIATE_FUNCTIONS_H

#include "Board_Struct.h"
#include "Player_Struct.h"

/**
* This function initiates the player's red board
*/
struct board* init_red_board();

/**
* This function initiates the player's blue board
*/
struct _board* init_blue_board();

/**
* This function initiates a player
*/
struct _player* init_player(int num_player, char* name);

#endif