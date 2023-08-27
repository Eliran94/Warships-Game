#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "Player_Struct.h"

#include "Constants.h"
#include "Deallocate_Memomry_Functions.h"
#include "Check_Functions.h"
#include "Initiate_Functions.h"
#include "Game_Functions.h"
#include "Game_On_Function.h"

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

	turn_player_num = dice_rolling_who_starts(player_1, player_2);
	printf("\nPress any key to start: ");
	getch();
	system("cls");

	while (check_if_player_alive(player_1) && check_if_player_alive(player_2))
	{
		player_turn(player_1, player_2, &turn_player_num);
	}

	if (!check_if_player_alive(player_1)) {
		printf("\nThe winner is %s! You are the real captain!\n", player_2->name);
	}
	else {
		printf("\nThe winner is %s! You are the real captain!\n", player_1->name);
	}

	free_player(player_1);
	free_player(player_2);
}