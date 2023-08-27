/**
* This header file declares the functions that put together the "Game_On_Function"
*/

#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "Board_Struct.h"
#include "Player_Struct.h"

/**
* This function prints the player's board
*/
void print_board(struct _board* b_game);

/**
* This function places a ship on the player's board
*/
void place_ship_on_board(struct _board* b_game, struct _ship* ship);

/**
* This function enables the player to place all ships on the board
*/
void place_ships(struct _player* player);

/**
* This function prints the game instructions from a text file
*/
void instructions_print(char* file_adress);

/**
* This function decides, by rolling a dice, which player starts the first turn - return 1 if player number 1 does,
* and return 2 if player number 2 does.
*/
int dice_rolling_who_starts(struct _player* player_1, struct _player* player_2);

/**
* This function enables the players to place theirs ships on board (it actually uses the "place_ships" function
*/
void boards_arrangement(struct _player* player_1, struct _player* player_2);

/**
* This function manages each player's turn
*/
void player_turn(struct _player* player_1, struct _player* player_2, int* turn_player_num);

/**
* This function enables the player to choose 4 coordinates to shell, shells accordingly, and presents
* the shell's result
*/
void attack(struct _player* attacker, struct _player* defender);


#endif