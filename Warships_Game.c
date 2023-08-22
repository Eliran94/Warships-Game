#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


#define MAX_NAME_LENGTH 30
#define BOARD_SIZE 10
#define NUM_SHIPS 10
#define MOTHER_SHIP 5
#define SUBMARINE 4
#define WARSHIP 3
#define COMBAT_SHIP 2

typedef struct _point
{
	int x;
	int y;
}point;

typedef struct _ship
{
	int type; //1 - mother ship, 2 - submarrine, 3 - warship, 4 - combat ship
	int ship_size; //5 - mother ship, 4 - submarrine, 3 - warship, 2 - combat ship
	int direction; //1 - horizontal, 2 - vertical
	point* position; //the first point is up or left corner
}ship;

typedef struct _board
{
	int** game_board;
	ship** ships;
}board;

typedef struct _player
{
	int num_player;
	char* name;
	board* blue_board;
	board* red_board;
	int lives;
}player;


void blue()
{
	printf("\033[1;34m");
}

void red()
{
	printf("\033[1;31m");
}

void reset()
{
	printf("\033[0m");
}

void free_red_board(board* b)
{
	int i;
	for (i = 0; i < BOARD_SIZE; i++)
		free(b->game_board[i]);
	free(b->game_board);
	free(b);
}

void free_ships(board* b, int i)
{
	int j;
	for (j = 0; j < i; j++)
	{
		free(b->ships[j]->position);
		free(b->ships[j]);
	}
	free(b->ships);
}

void free_blue_board(board* b)
{
	free_ships(b, NUM_SHIPS);
	free_red_board(b);
}

void free_player(player* player)
{
	free(player->name);
	free_red_board(player->red_board);
	free_blue_board(player->blue_board);
	free(player);
}

int check_ship_position(board* b_game,ship* ship, point position, int dir)
{
	int ship_size = 6 - ship->type;
	if (dir != 1 && dir != 2)
	{
		printf("Error: invalid diraction\n");
		return 0;
	}
	if (position.x < 1 || position.x > 12 || position.y < 1 || position.y > 12)
	{
		printf("Error: cordinates have to be between range of 1 to %d\n", BOARD_SIZE);
		return 0;
	}
	if (((position.x - 1 + ship_size) > BOARD_SIZE && dir == 2) ||((position.y - 1 + ship_size) > BOARD_SIZE && dir == 1))
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
		if (dir == 1)
			position.y++;
		else
			position.x++;
		ship_size--;
	}
	return 1;
}

int check_shoot(point shoot)
{
	if (shoot.x < 1 || shoot.x > 12 || shoot.y < 1 || shoot.y > 12)
	{
		printf("Error: cordinates have to be between range of 1 to %d\n", BOARD_SIZE);
		return 0;
	}
	return 1;
}

int check_player_alive(player* player) //1 - alive, 0 - all ships are destroyed
{
	int i, j, sum=0;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			if (player->blue_board->game_board[i][j] == 'X')
				sum++;
	if (player->lives == sum)
		return 0;
	return 1;
}

void place_ship_on_board(board* b_game, ship* ship)
{
	int ship_size = 6 - ship->type, dir_add = 0, i;
	for (i = 0; i < ship_size; i++)
	{
		if (ship->direction == 1)
		{
			ship->position[i].x = ship->position[0].x;
			ship->position[i].y = ship->position[0].y + dir_add;
			switch (i)
			{
			case 0:
				b_game->game_board[ship->position[i].x][ship->position[i].y] = '<';
				break;
			case 1:
				if (ship->type == 1)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
				else if (ship->type == 2)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'S';
				else if (ship->type == 3)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'W';
				else
					b_game->game_board[ship->position[i].x][ship->position[i].y] = '>';
				break;
			case 2:
				if (ship->type == 1)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
				else if (ship->type == 2)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'U';
				else if (ship->type == 3)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = '>';
				break;
			case 3:
				if (ship->type == 1)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
				else if (ship->type == 2)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = '>';
				break;
			case 4:
				b_game->game_board[ship->position[i].x][ship->position[i].y] = '>';
				break;
			}
		}
		else
		{
			ship->position[i].x = ship->position[0].x + dir_add;
			ship->position[i].y = ship->position[0].y;
			switch (i)
			{
			case 0:
				b_game->game_board[ship->position[i].x][ship->position[i].y] = '^';
				break;
			case 1:
				if (ship->type == 1)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
				else if (ship->type == 2)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'S';
				else if (ship->type == 3)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'W';
				else
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'V';
				break;
			case 2:
				if (ship->type == 1)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
				else if (ship->type == 2)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'U';
				else if (ship->type == 3)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'V';
				break;
			case 3:
				if (ship->type == 1)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'M';
				else if (ship->type == 2)
					b_game->game_board[ship->position[i].x][ship->position[i].y] = 'V';
				break;
			case 4:
				b_game->game_board[ship->position[i].x][ship->position[i].y] = 'V';
				break;
			}
		}
		dir_add++;
	}
}

board* init_red_board()
{
	int i, j;
	board* b_game = (board*)malloc(sizeof(board));
	if (b_game == NULL)
		return NULL;
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
			for (j = i - 1; j >= 0; j--)
				free(b_game->game_board[j]);
			free(b_game->game_board);
			free(b_game);
			return NULL;
		}
		for (j = 0; j < BOARD_SIZE; j++)
			b_game->game_board[i][j] = '~';
	}
	b_game->ships = NULL;
	return b_game;
}

board* init_blue_board()
{
	int i,j;
	board* b_game = init_red_board();
	if (b_game == NULL)
		return NULL;
	b_game->ships = (ship**)malloc(NUM_SHIPS * sizeof(ship*));
	if (b_game->ships == NULL)
	{
		free_red_board(b_game);
		return NULL;
	}
	ship* new_ship;
	for (i = 0; i < NUM_SHIPS; i++)
	{
		new_ship = (ship*)malloc(sizeof(ship));
		if (new_ship == NULL)
		{
			free_ships(b_game, i);
			free_red_board(b_game);
			return NULL;
		}
		if (i == 0)
		{
			new_ship->type = 1;
			new_ship->ship_size = MOTHER_SHIP;
			new_ship->position = (point*)malloc(MOTHER_SHIP*sizeof(point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}
		else if (i >= 1 && i <= 2)
		{
			new_ship->type = 2;
			new_ship->ship_size = SUBMARINE;
			new_ship->position = (point*)malloc(SUBMARINE* sizeof(point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}
		else if (i >= 3 && i <= 5)
		{
			new_ship->type = 3;
			new_ship->ship_size = WARSHIP;
			new_ship->position = (point*)malloc(WARSHIP* sizeof(point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}
		else
		{
			new_ship->type = 4;
			new_ship->ship_size = COMBAT_SHIP;
			new_ship->position = (point*)malloc(COMBAT_SHIP* sizeof(point));
			if (new_ship->position == NULL)
			{
				free(new_ship);
				free_ships(b_game, i);
				free_red_board(b_game);
				return NULL;
			}
		}
		new_ship->direction = 0;
		b_game->ships[i] = new_ship;
	}
	return b_game;
}

player* init_player(int num_player, char* name)
{
	int i;
	player* new_player = (player*)malloc(sizeof(player));
	if (new_player == NULL)
		return NULL;
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
	for (i = 0; i < NUM_SHIPS; i++)
		new_player->lives += new_player->blue_board->ships[i]->ship_size;
	return new_player;
}

void print_board(board* b_game)
{
	int i, j , s;
	if (b_game->ships == NULL)
		printf("\n---------The Opponent's Board---------\n");
	else
		printf("--------------Your Board--------------\n");
	for (i = 0; i < BOARD_SIZE+1; i++)
	{
		for (j = 0; j < BOARD_SIZE + 1; j++)
		{
			if (i == 0 && j == 0)
				printf("%3s", "X|Y");
			else if (i == 0 && j != 0)
				printf("%3d", j);
			else if (i != 0 && j == 0)
				printf("%3d", i);
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

void place_ships(player* player)
{
	point ship_position;
	int ship_dir = 0,i;
	printf("\n%s please place your ships on the board\n",player->name);
	for (i = 0; i < NUM_SHIPS; i++)
	{
		print_board(player->blue_board);
		if (i == 0)
			printf("First place your Mother-Ship\n");
		else if (i >= 1 && i <= 2)
			printf("\nPlace a Submarine\n");
		else if (i >= 3 && i <= 5)
			printf("\nPlace a Warship\n");
		else
			printf("\nPlace a Combat-ship\n");
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

void attack(player* attacker, player* defender)
{
	int i, cor_red, defender_blue_pos;
	point shoots[4];
	printf("%s choose 4 cordinates to shoot with your cannon\n",attacker->name);
	for (i = 0; i < 4; i++)
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
		else if (defender_blue_pos == 'X')
			continue;
		else
			attacker->red_board->game_board[shoots[i].x - 1][shoots[i].y-1] = 'O';
	}
	printf("\nThe results of the attack: \n");
	print_board(attacker->red_board);
}

void game_on()
{
	int turn_player_num = 0, dice;
	FILE* fp = fopen("game's_instructions.txt", "r");
	char line[200], name1[MAX_NAME_LENGTH], name2[MAX_NAME_LENGTH], even_odd[5];
	player* player_1, * player_2;
	time_t t1;
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
	place_ships(player_1);
	printf("%s, this is your board arrangement: \n", name1);
	print_board(player_1->blue_board);
	printf("\nTo finish press any key");
	getch();
	system("cls");
	place_ships(player_2);
	printf("%s, this is your board arrangement: \n", name2);
	print_board(player_2->blue_board);
	printf("\nTo finish press any key");
	getch();
	system("cls");
	printf("\nLets roll a dice to decide who begins. %s choose 'even' or 'odd': ",player_1->name);
	do
	{
		scanf("%s", even_odd);
		if ((strcmp(even_odd, "even")!=0 && strcmp(even_odd, "odd"))!=0)
			printf("Invalid choice. choose 'even' or 'odd': ");
	} while ((strcmp(even_odd, "even") != 0 && strcmp(even_odd, "odd")) != 0);
	srand((unsigned)time(&t1));
	dice = (rand() % 6) + 1;
	if ((dice % 2 == 0 && strcmp(even_odd, "even")==0) || (dice % 2 == 1 && strcmp(even_odd, "odd")==0))
		turn_player_num = 1;
	else
		turn_player_num = 2;
	if (turn_player_num == 1)
		printf("The dice shows: %d. %s you are first!\n",dice, name1);
	else
		printf("The dice shows: %d. %s you are first!\n", dice, name2);
	printf("\nPress any key to start: ");
	getch();
	system("cls");
	while (check_player_alive(player_1) && check_player_alive(player_2))
	{
		if (turn_player_num == 1)
		{
			printf("%s's turn\n", name1);
			printf("\nPress any key to start your turn: ");
			getch();
			system("cls");
			print_board(player_1->blue_board);
			print_board(player_1->red_board);
			attack(player_1, player_2);
			turn_player_num = 2;
			printf("\nPress any key to end your turn: ");
			getch();
			system("cls");
		}
		else
		{
			printf("%s's turn\n", name2);
			printf("\nPress any key to start your turn: ");
			getch();
			system("cls");
			print_board(player_2->blue_board);
			print_board(player_2->red_board);
			attack(player_2, player_1);
			turn_player_num = 1;
			printf("\nPress any key to end your turn: ");
			getch();
			system("cls");
		}
	}
	if (!check_player_alive(player_1))
		printf("\nThe winner is %s! You are the real captain!", name2);
	else
		printf("\nThe winner is %s! You are the real captain!", name1);
	free_player(player_1);
	free_player(player_2);
}

int main()
{
	game_on();
	return 0;
}