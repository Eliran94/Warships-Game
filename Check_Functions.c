#include <stdio.h>

#include "Point_Struct.h"
#include "Ship_Struct.h"
#include "Board_Struct.h"
#include "Player_Struct.h"

#include "Constants.h"
#include "Direction.h"

int check_valid_ship_position(struct _board* b_game, struct _ship* ship, struct _point position, enum direction dir)
{
	int ship_size = ship->ship_size;
	if (dir != HORIZONTAL && dir != VERTICAL)
	{
		printf("Error: invalid diraction\n");
		return 0;
	}
	if (position.x < 1 || position.x > 12 || position.y < 1 || position.y > 12)
	{
		printf("Error: cordinates have to be between range of 1 to %d\n", BOARD_SIZE);
		return 0;
	}
	if (((position.x - 1 + ship_size) > BOARD_SIZE && dir == VERTICAL) ||
		((position.y - 1 + ship_size) > BOARD_SIZE && dir == HORIZONTAL))
	{
		printf("Error: the ship exceeds the board limit\n");
		return 0;
	}
	while (ship_size != 0)
	{
		if (b_game->game_board[position.x - 1][position.y - 1] != '~')
		{
			printf("Error: ship position is occupied\n");
			return 0;
		}
		if (dir == HORIZONTAL) {
			position.y++;
		}
		else {
			position.x++;
		}
		ship_size--;
	}
	return 1;
}

int check_valid_shoot(struct _point shoot)
{
	if (shoot.x < 1 || shoot.x > 12 || shoot.y < 1 || shoot.y > 12)
	{
		printf("Error: cordinates have to be between range of 1 to %d\n", BOARD_SIZE);
		return 0;
	}
	return 1;
}

int check_if_player_alive(struct _player* player)
{
	int i, j, sum = 0;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (player->blue_board->game_board[i][j] == 'X') {
				sum++;
			}
		}
	}
	if (player->lives == sum) {
		return 0;
	}
	return 1;
}