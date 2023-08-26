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
#include "Deallocate_Memomry_Functions.h"


int check_ship_position(struct _board* b_game,struct _ship* ship, struct _point position, enum direction dir)
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

int check_shoot(struct _point shoot)
{
	if (shoot.x < 1 || shoot.x > 12 || shoot.y < 1 || shoot.y > 12)
	{
		printf("Error: cordinates have to be between range of 1 to %d\n", BOARD_SIZE);
		return 0;
	}
	return 1;
}

int check_player_alive(struct _player* player) //1 - alive, 0 - all ships are destroyed
{
	int i, j, sum=0;
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
	int i,j;
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
		if (i == 0)
		{
			new_ship->type = MOTHER_SHIP_TYPE;
			new_ship->ship_size = MOTHER_SHIP;
			new_ship->position = (struct _point*)malloc(MOTHER_SHIP*sizeof(struct _point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_prev_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}
		else if (i >= 1 && i <= 2)
		{
			new_ship->type = SUBMARINE_TYPE;
			new_ship->ship_size = SUBMARINE;
			new_ship->position = (struct _point*)malloc(SUBMARINE* sizeof(struct _point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_prev_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}
		else if (i >= 3 && i <= 5)
		{
			new_ship->type = WARSHIP_TYPE;
			new_ship->ship_size = WARSHIP;
			new_ship->position = (struct _point*)malloc(WARSHIP* sizeof(struct _point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_prev_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}
		else
		{
			new_ship->type = COMBAT_SHIP_TYPE;
			new_ship->ship_size = COMBAT_SHIP;
			new_ship->position = (struct _point*)malloc(COMBAT_SHIP* sizeof(struct _point));
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

void print_board(struct _board* b_game)
{
	int i, j , s;
	if (b_game->ships == NULL) {
		printf("\n---------The Opponent's Board---------\n");
	}
	else {
		printf("--------------Your Board--------------\n");
	}
	for (i = 0; i < BOARD_SIZE+1; i++)
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

void place_ships(struct _player* player)
{
	struct _point ship_position;
	enum direction ship_dir = NIL;
	printf("\n%s please place your ships on the board\n",player->name);
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
		} while (!check_ship_position(player->blue_board,player->blue_board->ships[i],ship_position,ship_dir));
		player->blue_board->ships[i]->position[0].x = (ship_position.x - 1);
		player->blue_board->ships[i]->position[0].y = (ship_position.y - 1);
		player->blue_board->ships[i]->direction = ship_dir;
		place_ship_on_board(player->blue_board, player->blue_board->ships[i]);
		system("cls");
	}
}

void attack(struct _player* attacker, struct _player* defender)
{
	int i, cor_red, defender_blue_pos;
	struct _point shoots[NUM_OF_SHOOTS];
	printf("%s choose %d cordinates to shoot with your cannon\n",attacker->name,NUM_OF_SHOOTS);
	for (i = 0; i < NUM_OF_SHOOTS; i++)
	{
		do
		{
			printf("Shoot number %d (Y,X): ", i+1);
			scanf("%d,%d", &shoots[i].x, &shoots[i].y);
		} while (!check_shoot(shoots[i]));
		defender_blue_pos = defender->blue_board->game_board[shoots[i].x - 1][shoots[i].y - 1];
		if (defender_blue_pos != '~' && defender_blue_pos != 'X')
		{
			attacker->red_board->game_board[shoots[i].x - 1][shoots[i].y-1] = 'X';
			defender->blue_board->game_board[shoots[i].x - 1][shoots[i].y-1] = 'X';
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

void game_on()
{
	int turn_player_num;
	char name1[MAX_NAME_LENGTH], name2[MAX_NAME_LENGTH];
	struct _player* player_1, * player_2;

	instructions_print("game's_instructions.txt");

	printf("\n\nFirst player name: ");
	scanf("%s", name1);
	printf("\nSecond player name: ");
	scanf("%s", name2);
	player_1 = init_player(1, name1);
	if (player_1 == NULL)
	{
		printf("Fatal memory allocation error");
		exit(1);
	}
	player_2 = init_player(2, name2);
	if (player_2 == NULL)
	{
		printf("Fatal memory allocation error");
		free_player(player_1);
		exit(1);
	}
	system("cls");

	boards_arrangement(player_1, player_2);

	//* Dice rolling to choose who starts */
	turn_player_num = dice_rolling_who_starts(player_1,player_2);
	printf("\nPress any key to start: ");
	getch();
	system("cls");

	while (check_player_alive(player_1) && check_player_alive(player_2))
	{
		player_turn(player_1, player_2, &turn_player_num);
	}
	if (!check_player_alive(player_1)) {
		printf("\nThe winner is %s! You are the real captain!\n", player_2->name);
	}
	else {
		printf("\nThe winner is %s! You are the real captain!\n", player_1->name);
	}
	free_player(player_1);
	free_player(player_2);
}

int main()
{
	game_on();
	return 0;
}
