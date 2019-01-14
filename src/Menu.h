#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "Button.h"

class Menu : public GameState
{
    public:

    
    LButton eButton;    
    LButton mButton;
    LButton hButton;

    LTexture ButtonSpriteSheet;

    LTexture titleText;
    LTexture eText;
    LTexture mText;
    LTexture hText;
    LTexture creditText;

    LTexture cautionBackdrop;
    LTexture menuPlate;

    SDL_Rect ButtonSpriteClips[ BUTTON_SPRITE_NUM ];

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
            eButton = LButton(SCREEN_WIDTH/4 - BUTTON_WIDTH/2, 200, BUTTON_WIDTH, BUTTON_HEIGHT);
        	mButton = LButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 200, BUTTON_WIDTH, BUTTON_HEIGHT);
            hButton = LButton(SCREEN_WIDTH*3/4 - BUTTON_WIDTH/2, 200, BUTTON_WIDTH, BUTTON_HEIGHT);

        	SDL_SetWindowSize(gWindow,SCREEN_WIDTH, SCREEN_HEIGHT);
        }

    }

    ///Deconstructor
    ~Menu(){
        printf("Menu Object Deconstructing...\n");

		//Destroy Menu assets here

        //Free loaded image
        ButtonSpriteSheet.free();
        titleText.free();
        eText.free();
        mText.free();
        hText.free();
        creditText.free();
        cautionBackdrop.free();
        menuPlate.free();

    }

    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        if( !cautionBackdrop.loadFromFile( "../assets/caution_background.png") )
        {
            printf( "Failed to load caution texture!\n" );
            success = false;
        }

        if( !menuPlate.loadFromFile( "../assets/menu_plate.png") )
        {
            printf( "Failed to load menu plate texture!\n" );
            success = false;
        }

        if( !ButtonSpriteSheet.loadFromFile( "../assets/button_SS.png") )
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
        //Open the font
        gFont = TTF_OpenFont( "../assets/PixelSplitter-Bold.ttf", 60 );
        if( gFont == NULL )
        {
            printf( "Failed to load Minesweeper Title font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            SDL_Color textColor = { 0, 0, 0 };
            if( !titleText.loadFromRenderedText( "Minesweeper", textColor ) )
            {
                printf( "Failed to render title text!\n" );
                success = false;
            }
        }

        gFont = TTF_OpenFont( "../assets/PressStart2P.ttf", 14 );
        if( gFont == NULL )
        {
            printf( "Failed to load Minesweeper font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            SDL_Color textColor = { 0, 0, 0 };
            if( !creditText.loadFromRenderedText( "By Joel Turner | Version: 1.0.0", textColor ) )
            {
                printf( "Failed to render credit text!\n" );
                success = false;
            }
        }

        gFont = TTF_OpenFont( "../assets/PressStart2P.ttf", 20 );
        if( gFont == NULL )
        {
            printf( "Failed to load Minesweeper font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            SDL_Color textColor = { 0, 0, 0 };
            if( !eText.loadFromRenderedText( "Easy", textColor ) )
            {
                printf( "Failed to render eText!\n" );
                success = false;
            }
            if( !mText.loadFromRenderedText( "Med", textColor ) )
            {
                printf( "Failed to render mText!\n" );
                success = false;
            }
            if( !hText.loadFromRenderedText( "Hard", textColor ) )
            {
                printf( "Failed to render hText!\n" );
                success = false;
            }
        }
        return success;
    }
    
    ///Handles mouse event
    void handleEvent( SDL_Event* e){

        //Handle button events first
        eButton.handleEvent(e);
        mButton.handleEvent(e);
        hButton.handleEvent(e);

        if (eButton.isClicked)
            set_next_state(STATE_GAME_EASY);

        if (mButton.isClicked)
            set_next_state(STATE_GAME_MED);

        if (hButton.isClicked)
            set_next_state(STATE_GAME_HARD);
    }

    void logic(){
    	eButton.logic();
        mButton.logic();
        hButton.logic();
    }

    void render(){
    	cautionBackdrop.render(0,0);
    	menuPlate.render(32,32);
        eButton.render(ButtonSpriteClips, &ButtonSpriteSheet);
        mButton.render(ButtonSpriteClips, &ButtonSpriteSheet);
        hButton.render(ButtonSpriteClips, &ButtonSpriteSheet);
        titleText.render(SCREEN_WIDTH/2 - titleText.getWidth()/2, 30);
        eText.render(SCREEN_WIDTH/4 - BUTTON_WIDTH/2 + eButton.mDimension.w/2 - eText.getWidth()/2, 200 + BUTTON_HEIGHT + 10);
        mText.render(SCREEN_WIDTH/2 - BUTTON_WIDTH/2 + mButton.mDimension.w/2 - mText.getWidth()/2, 200 + BUTTON_HEIGHT + 10);
        hText.render(SCREEN_WIDTH*3/4 - BUTTON_WIDTH/2 + hButton.mDimension.w/2 - hText.getWidth()/2, 200 + BUTTON_HEIGHT + 10);
        creditText.render(SCREEN_WIDTH/2 - creditText.getWidth()/2, SCREEN_HEIGHT - 60);
    }

};

#endif // MENU_H_INCLUDED