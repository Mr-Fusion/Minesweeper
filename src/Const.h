#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

//The window renderer
extern SDL_Renderer* gRenderer = NULL;

//The window we'll be rendering to
extern SDL_Window* gWindow = NULL;

//Globally used font
extern TTF_Font *gFont = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//const int BUTTON_WIDTH = 300;
//const int BUTTON_HEIGHT = 300;



const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

const int MENU_BAR_HEIGHT = TILE_HEIGHT * 2;

const int GRID_HEIGHT_EASY = 10;
const int GRID_WIDTH_EASY = 10;

const int GRID_HEIGHT_MED = 10;
const int GRID_WIDTH_MED = 20;

const int GRID_HEIGHT_HARD = 16;
const int GRID_WIDTH_HARD = 30;

const int SPRITE_NUM = 14;
const int SS_ROWS = 7;
const int SS_COLUMNS = 2;

const int SPRITE_HEIGHT = 32;
const int SPRITE_WIDTH = 32;

const int BUTTON_SPRITE_NUM = 4;
const int BUTTON_SS_ROWS = 2;
const int BUTTON_SS_COLUMNS = 2;

const int BUTTON_HEIGHT = 100;
const int BUTTON_WIDTH = 100;

const int MINE_NUM_EASY = 10;
const int MINE_NUM_MED = 40;
const int MINE_NUM_HARD = 99;

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
enum GameStates
{
    STATE_NULL,
    STATE_MENU,
    STATE_GAME_EASY,
    STATE_GAME_MED,
    STATE_GAME_HARD,
    STATE_EXIT
};

#endif // CONST_H_INCLUDED
