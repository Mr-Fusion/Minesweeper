#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

//The mouse button
class LButton
{
    public:

        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
        LButtonSprite mCurrentSprite;


        //Initializes internal variables
        LButton(){
            mPosition.x = 0;
            mPosition.y = 0;

            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }

        //Sets top left position
        void setPosition( int x, int y ){
            mPosition.x = x;
            mPosition.y = y;
        }

        //Handles mouse event
        void handleEvent( SDL_Event* e ){
            //If mouse event happened
            if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
            {
                //Get mouse position
                int x, y;
                SDL_GetMouseState( &x, &y );

                //Check if mouse is in button
                bool inside = true;

                //Mouse is left of the button
                if( x < mPosition.x )
                {
                    inside = false;
                }
                //Mouse is right of the button
                else if( x > mPosition.x + BUTTON_WIDTH )
                {
                    inside = false;
                }
                //Mouse above the button
                else if( y < mPosition.y )
                {
                    inside = false;
                }
                //Mouse below the button
                else if( y > mPosition.y + BUTTON_HEIGHT )
                {
                    inside = false;
                }

                //Mouse is outside button
                if( !inside )
                {
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                }
                //Mouse is inside button
                else
                {
                    //Set mouse over sprite
                    switch( e->type )
                    {
                        case SDL_MOUSEMOTION:
                        mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                        break;

                        case SDL_MOUSEBUTTONDOWN:
                        mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                        break;

                        case SDL_MOUSEBUTTONUP:
                        mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                        break;
                    }
                }
            }
        }

        //Shows button sprite
        void render(){
            //Show current button sprite
            gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
        }


};

#endif // BUTTON_H_INCLUDED
