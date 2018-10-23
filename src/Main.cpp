//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <ctime>

#include "Const.h"
#include "Grid.h"
#include "Minesweeper.h"
#include "LTexture.h"

/*GameState Stuff*/
//Game state base class
/*
class GameState
{
    public:
    virtual void handle_events() = 0;
    virtual void logic() = 0;
    virtual void render() = 0;
    virtual ~GameState(){};
};

//State status manager
void set_next_state( int newState );

//State changer
void change_state();

//State variables
int stateID = STATE_NULL;
int nextState = STATE_NULL;

//Game state object
GameState *currentState = NULL;

class Minesweeper : public GameState
{
    private:
    //Level dimensions
    const static int LEVEL_WIDTH = 1280;
    const static int LEVEL_HEIGHT = 960;

    //Overworld background
    SDL_Surface *background;

    //The houses
    House redHouse;
    House blueHouse;

    public:
    //Loads resources and initializes objects
    OverWorld( int prevState );

    //Frees resources
    ~OverWorld();

    //Main loop functions
    void handle_events();
    void logic();
    void render();
};

void change_state()
{
    //If the state needs to be changed
    if( nextState != STATE_NULL )
    {
        //Delete the current state
        if( nextState != STATE_EXIT )
        {
            delete currentState;
        }

        //Change the state
        switch( nextState )
        {
            case STATE_TITLE:
                currentState = new Title();
                break;

            case STATE_GREEN_OVERWORLD:
                currentState = new OverWorld( stateID );
                break;

            case STATE_RED_ROOM:
                currentState = new RedRoom();
                break;

            case STATE_BLUE_ROOM:
                currentState = new BlueRoom();
                break;
        }

        //Change the current state ID
        stateID = nextState;

        //NULL the next state ID
        nextState = STATE_NULL;
    }
}
*/
/*End GameState Stuff*/

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
extern SDL_Window* gWindow = NULL;

//Globally used font
extern TTF_Font *gFont = NULL;


bool init()
{
	//Initialization flag
	bool success = true;

	//Randomize rand seed
	srand(time(NULL));

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}

void close()
{

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        //Initialize Minesweeper Field
        Minesweeper mineField = Minesweeper();

		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//While application is running
/**-----[GAME LOOP START!]-----**/
		while( !quit )
		{
/**-----[INPUT EVENTS]-----**/
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                if ( !mineField.gameOver){
                    mineField.handleEvent( &e );
                }

            }
/**-----[LOGIC HANDLING]-----**/
            mineField.logic();
/**-----[RENDERING]-----**/
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            //Render Grid
            mineField.render();

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
	}

	//Free resources and close SDL
	close();

	return 0;
}
