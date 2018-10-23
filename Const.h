#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

//The window renderer
extern SDL_Renderer* gRenderer = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 990;
const int SCREEN_HEIGHT = 660;

const int TILE_WIDTH = 33;
const int TILE_HEIGHT = 33;

const int GRID_HEIGHT = 10;//16;
const int GRID_WIDTH = 10;//30;

const int SPRITE_NUM = 14;
const int SS_ROWS = 7;
const int SS_COLUMNS = 2;

const int SPRITE_HEIGHT = 33;
const int SPRITE_WIDTH = 33;

const int TOTAL_MINES = 10;//99;

enum{EMPTY,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,MINE,UNKNOWN,PRESS,FLAG,MISPLACED};

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

//Game states
/*enum GameStates
{
    STATE_NULL,
    STATE_MENU,
    STATE_GAME,
};*/


#endif // CONST_H_INCLUDED
