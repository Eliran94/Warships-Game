/**
* This header file declares functions that check data for the program
*/

#ifndef CHECK_FUNCTIONS_H
#define CHECK_FUNCTIONS_H

#include "Point_Struct.h"
#include "Ship_Struct.h"
#include "Board_Struct.h"
#include "Player_Struct.h"
#include "Direction.h"

/**
* This function checks the validation of the user input for the ship position
*/
int check_valid_ship_position(struct _board* b_game, struct _ship* ship, struct _point position, enum direction dir);

/**
* This function checks the validation of the user input for the shell coordinate
*/
int check_valid_shoot(struct _point shoot);

/**
* This function checks if the player's ships are all destroyed (return 1) or not (return 0)
*/
int check_if_player_alive(struct _player* player);

#endif