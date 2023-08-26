#include <stdlib.h>
#include "Board_Struct.h"
#include "Player_Struct.h"
#include "Constants.h"
#include "Deallocate_Memomry_Functions.h"

void free_red_board(struct _board* b)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		free(b->game_board[i]);
	}
	free(b->game_board);
	free(b);
}

void free_prev_rows(struct _board* b, int i)
{
	for (int j = 0; j < i; j++)
	{
		free(b->game_board[j]);
	}
	free(b->game_board);
}

void free_prev_ships(struct _board* b, int i)
{
	for (int j = 0; j < i; j++)
	{
		free(b->ships[j]->position);
		free(b->ships[j]);
	}
	free(b->ships);
}

void free_blue_board(struct _board* b)
{
	free_prev_ships(b, NUM_SHIPS);
	free_red_board(b);
}

void free_player(struct _player* player)
{
	free(player->name);
	free_red_board(player->red_board);
	free_blue_board(player->blue_board);
	free(player);
}