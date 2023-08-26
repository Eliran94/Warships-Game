#ifndef SHIP_STRUCT_H
#define SHIP_STRUCT_H

#include "Ship_Type.h"
#include "Direction.h"
#include "Point_Struct.h"

typedef struct _ship
{
	enum type type;
	int ship_size;			   //* As in Constants.h
	enum direction direction;
	struct _point* position;   //* Pointer to the first up or left corner
};

#endif