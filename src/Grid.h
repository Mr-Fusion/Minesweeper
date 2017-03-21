#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <SDL.h>
#include "LTexture.h"
#include "Const.h"

class Grid
{
    public:

        //Grid location
        int xLoc;
        int yLoc;
        //Grid Dimensions
        int rMax;
        int rHeight;
        int cMax;
        int cWidth;
        //Current selected row/column
        int row;
        int col;

        //Grid array
        int **tiles;

        LTexture spriteSheet;

        //Initialize and create grid
        Grid(int c, int r, int cw, int rh, LTexture ss){
            rMax = r;
            cMax = c;
            spriteSheet = ss;
            setDim(cw,rh);
            setPos(0,0);


            tiles = new int *[cMax];
            for (int i = 0; i < cMax; i++)
                tiles[i] = new int[rMax];
            for (int i = 0; i < cMax; i++) {
                for (int j = 0; j < rMax; j++)
                    tiles[i][j] = 0;
            }

            selectTile(0,0);
        }

        ~Grid(){}

		//Set Grid Position
		void setPos(int x, int y){
		    xLoc = x;
            yLoc = y;
		}

		//Set Grid Dimensions
		void setDim(int cw, int rh){
            cWidth = cw;
            rHeight = rh;
		}

		//check
		bool isInside(int x, int y){
            //Mouse is left of the button
            if( x < xLoc )
            {
                return false;
            }
            //Mouse is right of the button
            else if( x > xLoc + cMax * cWidth )
            {
                return false;
            }
            //Mouse above the button
            else if( y < yLoc )
            {
                return false;
            }
            //Mouse below the button
            else if( y > yLoc + rMax * rHeight )
            {
                return false;
            }
            else
                return true;
		}

		//Select Grid Coordinate
		void selectTile( int x, int y ){
		    col = (x - xLoc) / cWidth;
            row = (y - yLoc) / rHeight;
		}

		//Set Tile sprite
		void setCurTile(int n){
		    tiles[col][row] = n;
		}

		//Handles mouse event
		void handleEvent( SDL_Event* e){
		    int x, y;
            //If mouse event happened
            if( e->type == SDL_MOUSEMOTION ){
                //Get mouse position
                SDL_GetMouseState( &x, &y );
                if (isInside(x,y))
                    selectTile( x, y );
            }
		}

		//Render the state of the playing grid
        void render( SDL_Rect sprite[]){
            for (int i = 0; i < cMax; i++)
            {
                for (int j = 0; j < rMax; j++)
                    spriteSheet.render( i * cWidth + xLoc, j * rHeight + yLoc, &sprite[tiles[i][j]]);
            }
        }

        //Render specific tile of the grid
        /*void renderTile(int x, int y, LTexture texture, SDL_Rect* sprite, int clip){
            x = x * cWidth + xLoc;
            y = y * rHeight + yLoc;
            texture.render( x, y, &sprite[ clip ] );
        }
        */
};

#endif // GRID_H_INCLUDED
