#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

// REMARKS: Defining all the desired properties of the button is overwhelming!!! Let's write out what we want:

// Desired Behavior/Logic:
// Button only activates if the mouse was pressed and released within the bounds of the button
// If pressed inside, but released outside, no activation
// If pressed outside, but released inside, no activation
// If pressed and release outside, no activation (obviously)
// If the cursor leaves the button after the press but returns before the release, that's an activation.

// Desired Graphical:
// Subtle highlight when mouse is hovering over (no click)
// Heavy highlight when mouse was pressed in bounds, and is currently within bounds
// No highlight (Default Texture) when mouse is outside bounds, or if mouse was pressed outside and has moved inside

// Spritesheet: Default, highlight, press (3 sprites)

// Functions (rough): 

//Hmmm... that's it. (For now!)

//The mouse button

enum ButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_HOVER = 1,
    BUTTON_SPRITE_MOUSE_OUT_PRESS = 2,
    BUTTON_SPRITE_MOUSE_HOVER_PRESS = 3,
    BUTTON_SPRITE_TOTAL = 4
};

class LButton
{
    public:

        //Top left position and dimensions
        SDL_Rect mDimension;

        //Currently used global sprite
        ButtonSprite mCurrentSprite;

        //LTexture spriteSheet;

        
        bool press;
        bool inside;
        bool isClicked;


        //Initializes internal variables
        LButton(){
            mDimension.x = 0;
            mDimension.y = 0;
            mDimension.w = 0;
            mDimension.h = 0;

            inside = false;
            press = false;
            isClicked = false;

            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }

                //Initializes internal variables
        LButton(int xPos,int yPos,int w,int h){
            mDimension.x = xPos;
            mDimension.y = yPos;
            mDimension.w = w;
            mDimension.h = h;

            inside = false;
            press = false;
            isClicked = false;

            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }

        ///Deconstructor
        ~LButton(){
            printf("Button Object Deconstructing...\n");

            //Destroy button assets here

        }

        //Sets top left position
        void setPosition( int x, int y ){
            mDimension.x = x;
            mDimension.y = y;
        }

        //Sets dimensions
        void setDimension( int w, int h ){
            mDimension.w = w;
            mDimension.h = h;
        }

        //Handles mouse event
        void handleEvent( SDL_Event* e ){
            //If mouse event happened
            if( e->type == SDL_MOUSEMOTION )
            {
                //Get mouse position
                int x, y;
                SDL_GetMouseState( &x, &y );

                //Check if mouse is in button
                inside = true;

                //Mouse is left of the button
                if( x < mDimension.x )
                {
                    inside = false;
                }
                //Mouse is right of the button
                else if( x > mDimension.x + mDimension.w )
                {
                    inside = false;
                }
                //Mouse above the button
                else if( y < mDimension.y )
                {
                    inside = false;
                }
                //Mouse below the button
                else if( y > mDimension.y + mDimension.h )
                {
                    inside = false;
                }
            }

            if (e->type == SDL_MOUSEBUTTONDOWN) {
                if (inside) {
                    press = true;
                }
            } 

            if ( e->type == SDL_MOUSEBUTTONUP  ) {
                if (inside) {
                    if (press) {
                        isClicked = true;
                    }
                }
                press = false;
            }
        }

        //Do Stuff
        void logic(){
            if (inside) {
                mCurrentSprite = BUTTON_SPRITE_MOUSE_HOVER;
                if (press) {
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_HOVER_PRESS;
                }
            }
            else {
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                if (press) {
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT_PRESS;
                }
            }

            if (isClicked) {

                //Do something...?

                //Then reset button
                isClicked = false;
            }
        }

        //Shows button sprite
        void render(SDL_Rect sprite[], LTexture *SpriteSheet){
            //Show current button sprite
            SpriteSheet->render( mDimension.x, mDimension.y, &sprite[ mCurrentSprite ] );
        }


};

#endif // BUTTON_H_INCLUDED
