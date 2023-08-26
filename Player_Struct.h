#ifndef PLAYER_STRUCT_H
#define PLAYER_STRUCT_H

#include "Board_Struct.h"

typedef struct _player
{
	int num_player;
	char* name;
	struct _board* blue_board;
	struct _board* red_board;
	int lives;
};

#endif