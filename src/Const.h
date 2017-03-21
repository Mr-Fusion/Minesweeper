#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 560;

const int TILE_WIDTH = 33;
const int TILE_HEIGHT = 33;

const int GRID_HEIGHT = 9;
const int GRID_WIDTH = 9;

const int SPRITE_NUM = 14;
const int SS_ROWS = 7;
const int SS_COLUMNS = 2;

const int SPRITE_HEIGHT = 33;
const int SPRITE_WIDTH = 33;

const int TOTAL_MINES = 10;

enum{EMPTY,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,MINE,UNKNOWN,PRESS,FLAG,WRONG};

enum direction
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
    NORTHEAST = 4,
    SOUTHEAST = 5,
    SOUTHWEST = 6,
    NORTHWEST = 7
};

typedef struct _Direction_Vector{
  int yDif, xDif;
} DirectionVector;

DirectionVector DIRECTIONS[] = {
    {-1,0},
    {0,1},
    {1,0},
    {0,-1},
    {-1,1},
    {1,1},
    {1,-1},
    {-1,-1}
};


#endif // CONST_H_INCLUDED
