//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <ctime>

#include "Const.h"
#include "Grid.h"
#include "Minesweeper.h"
#include "LTexture.h"

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
extern SDL_Window* gWindow = NULL;

//Globally used font
extern TTF_Font *gFont = NULL;

//The window renderer
extern SDL_Renderer* gRenderer = NULL;

//Test Sprite Sheet
LTexture gSpriteSheetTexture;

//Test Sprite Clips
SDL_Rect gSpriteClips[ SPRITE_NUM ];


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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load board texture
	/*
	if( !gBoardTexture.loadFromFile( "assets/Connect4Board.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	*/

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( gRenderer, "assets/minesweeper_tiles_3.png") )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
	    int n = 0;
	    for (int i = 0; i < SS_COLUMNS; i++){
            for (int j = 0; j < SS_ROWS; j++){
                gSpriteClips[ n ].x = SPRITE_WIDTH * i;
                gSpriteClips[ n ].y = SPRITE_HEIGHT * j;
                gSpriteClips[ n ].w = SPRITE_WIDTH;
                gSpriteClips[ n ].h = SPRITE_HEIGHT;
                n++;
            }
	    }
	}
	return success;
}

void close()
{

    //Free loaded image
	gSpriteSheetTexture.free();

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
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
            //Initialize Minesweeper Field
            Minesweeper mineField = Minesweeper(gSpriteSheetTexture);

			//Main loop flag
			bool quit = false;

			//Gameplay Enable flag
			bool gameOver = false;

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
                    if ( !gameOver){
                        //testGrid.handleEvent( &e );
                        mineField.handleEvent( &e );
                    }

                }
/**-----[LOGIC HANDLING]-----**/
                if (mineField.sweepEnable){
                    if ( !mineField.sweepTile(mineField.playField.col, mineField.playField.row) ){
                        printf("YOU LOSE...\n");
                        gameOver = true;
                    }
                    else{
                        if ( mineField.unexploredTiles == mineField.mines){
                            printf("YOU WIN!!!\n");
                            gameOver = true;
                        }
                    }
                    mineField.sweepEnable = false;
                }
                if (mineField.flagEnable){
                    mineField.mineFlagToggle(mineField.playField.col, mineField.playField.row);
                    mineField.flagEnable = false;
                }
/**-----[RENDERING]-----**/
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                //Render Grid
                mineField.render( gSpriteClips );

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
