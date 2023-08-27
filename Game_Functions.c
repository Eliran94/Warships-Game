#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//* Structions:
#include "Point_Struct.h"
#include "Ship_Struct.h"
#include "Board_Struct.h"
#include "Player_Struct.h"

#include "Constants.h"
#include "Colors.h"
#include "Direction.h"
#include "Ship_Type.h"
#include "Check_Functions.h"
#include "Game_Functions.h"

void print_board(struct _board* b_game)
{
	int i, j, s;
	if (b_game->ships == NULL) {
		printf("\n---------The Opponent's Board---------\n");
	}
	else {
		printf("--------------Your Board--------------\n");
	}
	for (i = 0; i < BOARD_SIZE + 1; i++)
	{
		for (j = 0; j < BOARD_SIZE + 1; j++)
		{
			if (i == 0 && j == 0) {
				printf("%3s", "X|Y");
			}
			else if (i == 0 && j != 0) {
				printf("%3d", j);
			}
			else if (i != 0 && j == 0) {
				printf("%3d", i);
			}
			else
			{
				s = b_game->game_board[i - 1][j - 1];
				if (s == '~')
					blue();
				if (s == 'X')
					red();
				printf("%3c", s);
				if (s == '~' || s == 'X')
					reset();
			}
		}
		printf("\n");
	}
}

void place_ship_on_board(struct _board* b_game, struct _ship* ship)
{
	enum direction dir_add = NIL;
	int start_ship, end_ship;
	if (ship->direction == HORIZONTAL) {
		start_ship = '<';
		end_ship = '>';
	}
	else {
		start_ship = '^';
		end_ship = 'V';
	}
	for (size_t i = 0; i < ship->ship_size; i++)
	{
		if (ship->direction == HORIZONTAL) {
			ship->position[i].x = ship->position[0].x;
			ship->position[i].y = ship->position[0].y + dir_add;
		}
		else {
			ship->position[i].x = ship->position[0].x + dir_add;
			ship->position[i].y = ship->position[0].y;
		}
		switch (i)
		{
		case 0:
			b_game->game_board[ship->position[i].x][ship->position[i].y] = start_ship;
			break;

		case 1:
			if (ship->type == MOTHER_SHIP_TYPE) {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
			}
			else if (ship->type == SUBMARINE_TYPE) {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = 'S';
			}
			else if (ship->type == WARSHIP_TYPE) {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = 'W';
			}
			else {

				b_game->game_board[ship->position[i].x][ship->position[i].y] = end_ship;
			}
			break;

		case 2:
			if (ship->type == MOTHER_SHIP_TYPE) {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
			}
			else if (ship->type == SUBMARINE_TYPE) {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = 'U';
			}
			else {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = end_ship;
			}
			break;

		case 3:
			if (ship->type == MOTHER_SHIP_TYPE) {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
			}
			else {
				b_game->game_board[ship->position[i].x][ship->position[i].y] = end_ship;
			}
			break;

		case 4:
			b_game->game_board[ship->position[i].x][ship->position[i].y] = end_ship;
			break;
		}
		dir_add++;
	}
}

void place_ships(struct _player* player)
{
	struct _point ship_position;
	enum direction ship_dir = NIL;
	printf("\n%s please place your ships on the board\n", player->name);
	for (size_t i = 0; i < NUM_SHIPS; i++)
	{
		print_board(player->blue_board);
		if (i == 0) {
			printf("First place your Mother-Ship\n");
		}
		else if (i >= 1 && i <= 2) {
			printf("\nPlace a Submarine\n");
		}
		else if (i >= 3 && i <= 5) {
			printf("\nPlace a Warship\n");
		}
		else {
			printf("\nPlace a Combat-ship\n");
		}
		do
		{
			printf("Choose the up/left corner of the ship (X,Y): ");
			scanf("%d,%d", &ship_position.x, &ship_position.y);
			printf("Choose diraction, 1 for horizontal, 2 for vertical: ");
			scanf("%d", &ship_dir);
		} while (!check_valid_ship_position(player->blue_board, player->blue_board->ships[i], ship_position, ship_dir));
		player->blue_board->ships[i]->position[0].x = (ship_position.x - 1);
		player->blue_board->ships[i]->position[0].y = (ship_position.y - 1);
		player->blue_board->ships[i]->direction = ship_dir;
		place_ship_on_board(player->blue_board, player->blue_board->ships[i]);
		system("cls");
	}
}

void instructions_print(char* file_adress)
{
	FILE* fp = fopen(file_adress, "r");
	char line[200];
	if (fp == NULL)
	{
		printf("Couldn't open the file");
		exit(1);
	}
	while (!feof(fp))
	{
		fgets(line, 200, fp);
		printf("%s", line);
	}
	fclose(fp);
}

int dice_rolling_who_starts(struct _player* player_1, struct _player* player_2)
{
	int turn_player_num = 0;
	char even_odd[10];
	time_t t1;
	printf("\nLets roll a dice to decide who begins. %s choose 'even' or 'odd': ", player_1->name);
	do
	{
		scanf("%s", even_odd);
		if ((strcmp(even_odd, "even") != 0 && strcmp(even_odd, "odd")) != 0) {
			printf("Invalid choice. choose 'even' or 'odd': ");
		}
	} while ((strcmp(even_odd, "even") != 0 && strcmp(even_odd, "odd")) != 0);
	srand((unsigned)time(&t1));
	int dice = (rand() % 6) + 1;
	if ((dice % 2 == 0 && strcmp(even_odd, "even") == 0) ||
		(dice % 2 == 1 && strcmp(even_odd, "odd") == 0)) {
		turn_player_num = 1;
	}
	else {
		turn_player_num = 2;
	}
	if (turn_player_num == 1) {
		printf("The dice shows: %d. %s you are first!\n", dice, player_1->name);
		return 1;
	}
	else {
		printf("The dice shows: %d. %s you are first!\n", dice, player_2->name);
		return 2;
	}
}

void boards_arrangement(struct _player* player_1, struct _player* player_2)
{
	place_ships(player_1);
	printf("%s, this is your board arrangement: \n", player_1->name);
	print_board(player_1->blue_board);
	printf("\nTo finish press any key");
	getch();
	system("cls");
	place_ships(player_2);
	printf("%s, this is your board arrangement: \n", player_2->name);
	print_board(player_2->blue_board);
	printf("\nTo finish press any key");
	getch();
	system("cls");
}

void player_turn(struct _player* player_1, struct _player* player_2, int* turn_player_num)
{
	if (*turn_player_num == 1)
	{
		printf("%s's turn\n", player_1->name);
		printf("\nPress any key to start your turn: ");
		getch();
		system("cls");
		print_board(player_1->blue_board);
		print_board(player_1->red_board);
		attack(player_1, player_2);
		*turn_player_num = 2;
		printf("\nPress any key to end your turn: ");
		getch();
		system("cls");
	}
	else
	{
		printf("%s's turn\n", player_2->name);
		printf("\nPress any key to start your turn: ");
		getch();
		system("cls");
		print_board(player_2->blue_board);
		print_board(player_2->red_board);
		attack(player_2, player_1);
		*turn_player_num = 1;
		printf("\nPress any key to end your turn: ");
		getch();
		system("cls");
	}
}

void attack(struct _player* attacker, struct _player* defender)
{
	int i, cor_red, defender_blue_pos;
	struct _point shoots[NUM_OF_SHOOTS];
	printf("%s choose %d cordinates to shoot with your cannon\n", attacker->name, NUM_OF_SHOOTS);
	for (i = 0; i < NUM_OF_SHOOTS; i++)
	{
		do
		{
			printf("Shoot number %d (Y,X): ", i + 1);
			scanf("%d,%d", &shoots[i].x, &shoots[i].y);
		} while (!check_valid_shoot(shoots[i]));
		defender_blue_pos = defender->blue_board->game_board[shoots[i].x - 1][shoots[i].y - 1];
		if (defender_blue_pos != '~' && defender_blue_pos != 'X')
		{
			attacker->red_board->game_board[shoots[i].x - 1][shoots[i].y - 1] = 'X';
			defender->blue_board->game_board[shoots[i].x - 1][shoots[i].y - 1] = 'X';
		}
		else if (defender_blue_pos == 'X') {
			continue;
		}
		else {
			attacker->red_board->game_board[shoots[i].x - 1][shoots[i].y - 1] = 'O';
		}
	}
	printf("\nThe results of the attack: \n");
	print_board(attacker->red_board);
}