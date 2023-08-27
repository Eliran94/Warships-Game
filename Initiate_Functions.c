#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Point_Struct.h"
#include "Ship_Struct.h"
#include "Board_Struct.h"
#include "Player_Struct.h"

#include "Constants.h"
#include "Ship_Type.h"
#include "Direction.h"
#include "Deallocate_Memomry_Functions.h"
#include "Initiate_Functions.h"

struct board* init_red_board()
{
	int i, j;
	struct _board* b_game = (struct _board*)malloc(sizeof(struct _board));
	if (b_game == NULL) {
		return NULL;
	}

	b_game->game_board = (int**)malloc(BOARD_SIZE * sizeof(int*));
	if (b_game->game_board == NULL)
	{
		free(b_game);
		return NULL;
	}

	for (i = 0; i < BOARD_SIZE; i++)
	{
		b_game->game_board[i] = (int*)calloc(BOARD_SIZE, sizeof(int));
		if (b_game->game_board[i] == NULL)
		{
			free_prev_rows(b_game, i);
			free(b_game);
			return NULL;
		}
		for (j = 0; j < BOARD_SIZE; j++) {
			b_game->game_board[i][j] = '~';
		}
	}

	b_game->ships = NULL;

	return b_game;
}

struct _board* init_blue_board()
{
	int i, j;
	struct _board* b_game = init_red_board();
	if (b_game == NULL) {
		return NULL;
	}
	b_game->ships = (struct _ship**)malloc(NUM_SHIPS * sizeof(struct _ship*));
	if (b_game->ships == NULL)
	{
		free_red_board(b_game);
		return NULL;
	}
	struct _ship* new_ship;
	for (i = 0; i < NUM_SHIPS; i++)
	{
		new_ship = (struct _ship*)malloc(sizeof(struct _ship));
		if (new_ship == NULL)
		{
			free_prev_ships(b_game, i);
			free_red_board(b_game);
			return NULL;
		}

		if (i == 0) //* Initiate mother-ship */
		{
			new_ship->type = MOTHER_SHIP_TYPE;
			new_ship->ship_size = MOTHER_SHIP;
			new_ship->position = (struct _point*)malloc(MOTHER_SHIP * sizeof(struct _point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_prev_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}

		else if (i >= 1 && i <= 2) //* Initiate submarines */
		{
			new_ship->type = SUBMARINE_TYPE;
			new_ship->ship_size = SUBMARINE;
			new_ship->position = (struct _point*)malloc(SUBMARINE * sizeof(struct _point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_prev_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}

		else if (i >= 3 && i <= 5) //* Initiate warships */
		{
			new_ship->type = WARSHIP_TYPE;
			new_ship->ship_size = WARSHIP;
			new_ship->position = (struct _point*)malloc(WARSHIP * sizeof(struct _point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_prev_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}

		else //* Initiate combat-ships */
		{
			new_ship->type = COMBAT_SHIP_TYPE;
			new_ship->ship_size = COMBAT_SHIP;
			new_ship->position = (struct _point*)malloc(COMBAT_SHIP * sizeof(struct _point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_prev_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}

		new_ship->direction = NIL;
		b_game->ships[i] = new_ship;
	}
	return b_game;
}

struct _player* init_player(int num_player, char* name)
{
	struct _player* new_player = (struct _player*)malloc(sizeof(struct _player));
	if (new_player == NULL) {
		return NULL;
	}

	new_player->name = (char*)malloc(MAX_NAME_LENGTH * sizeof(char));
	if (new_player->name == NULL)
	{
		free(new_player);
		return NULL;
	}

	new_player->red_board = init_red_board();
	if (new_player->red_board == NULL)
	{
		free(new_player->name);
		free(new_player);
		return NULL;
	}

	new_player->blue_board = init_blue_board();
	if (new_player->blue_board == NULL)
	{
		free_red_board(new_player->red_board);
		free(new_player->name);
		free(new_player);
		return NULL;
	}

	strcpy(new_player->name, name);
	new_player->num_player = num_player;
	new_player->lives = 0;
	for (int i = 0; i < NUM_SHIPS; i++) {
		new_player->lives += new_player->blue_board->ships[i]->ship_size;
	}

	return new_player;
}