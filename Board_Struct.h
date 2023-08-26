#ifndef BOARD_STRUCT_H
#define BOARD_STRUCT_H

#include "Ship_Struct.h"

typedef struct _board
{
	int** game_board;
	struct _ship** ships;
};

#endif