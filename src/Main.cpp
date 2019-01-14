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
#include "GameState.h"
#include "Menu.h"

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

GameState *currentState = NULL;

/*GameState Stuff*/

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
            case STATE_MENU:
                currentState = new Menu();
                break;

            case STATE_GAME_EASY:
                currentState = new Minesweeper(GRID_WIDTH_EASY, GRID_HEIGHT_EASY, MINE_NUM_EASY);
                break;

            case STATE_GAME_MED:
                currentState = new Minesweeper(GRID_WIDTH_MED, GRID_HEIGHT_MED, MINE_NUM_MED);
                break;

            case STATE_GAME_HARD:
                currentState = new Minesweeper(GRID_WIDTH_HARD, GRID_HEIGHT_HARD, MINE_NUM_HARD);
                break;
        }

        //Change the current state ID
        stateID = nextState;

        //NULL the next state ID
        nextState = STATE_NULL;
    }
}
/*End GameState Stuff*/

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
	delete currentState;

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
    	//Set the current state ID
    	stateID = STATE_MENU;

        //Initialize Minesweeper Field
        currentState = new Menu();

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
                if ( !currentState->gameOver){
                    currentState->handleEvent( &e );
                }

            }
/**-----[LOGIC HANDLING]-----**/
            currentState->logic();
        	
        	//Change state if needed
        	change_state();
        	
/**-----[RENDERING]-----**/
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            //Render Grid
            currentState->render();

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
	}

	//Free resources and close SDL
	close();

	return 0;
}
