#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "Button.h"

class Menu : public GameState
{
    public:

    LButton testButton;

    //LTexture tileset;
    LTexture ButtonSpriteSheet;

    //Test Sprite Clips
    SDL_Rect ButtonSpriteClips[ SPRITE_NUM ];

    ///Constructor Function
    Menu(){

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
        	//Initialize Menu here
        	testButton = LButton();
        	testButton.setPosition(300,300);
        	testButton.spriteSheet = ButtonSpriteSheet;
        }

    }

    ///Deconstructor
    ~Menu(){
        printf("Menu Object Deconstructing...\n");

		//Destroy Menu assets here

        //Free loaded image
        ButtonSpriteSheet.free();

    }

    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        if( !ButtonSpriteSheet.loadFromFile( "assets/button_SS.png") )
        {
            printf( "Failed to load sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < BUTTON_SS_COLUMNS; i++){
                for (int j = 0; j < BUTTON_SS_ROWS; j++){
                    ButtonSpriteClips[ n ].x = BUTTON_WIDTH * i;
                    ButtonSpriteClips[ n ].y = BUTTON_HEIGHT * j;
                    ButtonSpriteClips[ n ].w = BUTTON_WIDTH;
                    ButtonSpriteClips[ n ].h = BUTTON_HEIGHT;
                    n++;
                }
            }
        }
        return success;
    }
    
    ///Handles mouse event
    void handleEvent( SDL_Event* e){

        //Handle button events first
        testButton.handleEvent(e);



        if (testButton.isClicked){
            //Something about state change flags here
    		printf("CHANGE STATE\n");
            set_next_state(STATE_GAME);
        }
    }

    void logic(){
    	//Handle actual state change here?
    	testButton.logic();
    }

    void render(){
        testButton.render(ButtonSpriteClips/*, ButtonSpriteSheet*/);
    }

};

#endif // MENU_H_INCLUDED