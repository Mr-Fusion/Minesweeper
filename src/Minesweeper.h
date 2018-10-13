#ifndef MINESWEEPER_H_INCLUDED
#define MINESWEEPER_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include "Const.h"
#include "Grid.h"

class Minesweeper
{
    public:

    int row;
    int col;

    int mines;
    int unexploredTiles;

    int **fieldVisible;
    int **fieldHidden;

    bool sweepEnable;
    bool flagEnable;
    bool pressed;
    bool inPlay;

    //LTexture tileset;
    Grid playField;

    ///Constructor Function
    Minesweeper( LTexture t){

        //Initialize playing field dimensions, difficulty, and appearance
        col = GRID_WIDTH;
        row = GRID_HEIGHT;
        mines = TOTAL_MINES;
        playField.spriteSheet = t;
        playField.setPos(0,66);
        //tileset = t;



        //Initialize unexplored tile counter and event flags
        unexploredTiles = row * col;
        sweepEnable = false;
        flagEnable = false;
        inPlay = false;


        //Initialize visible playing field
        fieldVisible = new int *[col];
        for (int i = 0; i < col; i++)
            fieldVisible[i] = new int[row];

        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++)
                fieldVisible[i][j] = UNKNOWN;
        }

        //Initialize hidden playing field
        fieldHidden = new int *[col];
        for (int i = 0; i < col; i++)
            fieldHidden[i] = new int[row];

        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++)
                fieldHidden[i][j] = EMPTY;
        }

        //Hide mines at random locations
        int n = 0;
        while (n < mines) {
            int i = rand() % col;
            int j = rand() % row;
            if (fieldHidden[i][j] != MINE) {
                fieldHidden[i][j] = MINE;
                n++;
            }
        }

        //Generate clues
        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                if (fieldHidden[i][j] != MINE)
                    fieldHidden[i][j] = setClue(i,j);
            }
        }

        //Initialize and display graphical interface
        update();

    }

    ///Deconstructor
    ~Minesweeper(){
        printf("Minesweeper Object Deconstructing...");

        for(int i = 0; i < col; ++i) {
            delete [] fieldVisible[i];
        }
        delete [] fieldVisible;

        for(int i = 0; i < col; ++i) {
            delete [] fieldHidden[i];
        }
        delete [] fieldHidden;
    }

    ///Revert all graphical modifications to the playfield
    void update(void){
        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                playField.tiles[i][j] = fieldVisible[i][j];
            }
        }
    }

    ///Returns the number of mine tiles which are adjacent to the input coordinates
    int setClue(int x, int y){
        int xTemp = 0;
        int yTemp = 0;
        int clue = 0;
        DirectionVector dir;

        for (int d = 0; d < 9; d++){
            dir = DIRECTIONS[d];
            xTemp = x + dir.xDif;
            yTemp = y + dir.yDif;
            if ( xTemp >= 0 && xTemp < col && yTemp >= 0 && yTemp < row){
                if (fieldHidden[xTemp][yTemp] == MINE)
                    clue++;
            }
        }
        return clue;
    }

    ///Reveals the identity of the tile at the input coordinates
    ///If the revealed tile is empty, reveal all adjacent tiles
    ///Tiles marked with a flag cannot be revealed
    ///If a mine is revealed, the player dies and the game is over
    bool sweepTile(int x, int y){
        bool alive = true;
        int xTemp = 0;
        int yTemp = 0;
        DirectionVector dir;

        if ( !(fieldVisible[x][y] == FLAG) ){
            fieldVisible[x][y] = fieldHidden[x][y];
            unexploredTiles--;
            if ( fieldVisible[x][y] == EMPTY){
                for (int d = 0; d < 9; d++){
                    dir = DIRECTIONS[d];
                    xTemp = x + dir.xDif;
                    yTemp = y + dir.yDif;
                    if ( xTemp >= 0 && xTemp < col && yTemp >= 0 && yTemp < row){
                        if (fieldVisible[xTemp][yTemp] == UNKNOWN)
                            sweepTile( xTemp, yTemp );
                    }
                }
            }
            if ( fieldVisible[x][y] == MINE ){
                alive = false;
                revealAll();
            }
        }
        update();
        return alive;
    }

    ///Toggles flag marker (Unknown tiles only)
    void mineFlagToggle(int x, int y){
        if (fieldVisible[x][y] == FLAG)
            fieldVisible[x][y] = UNKNOWN;
        else if (fieldVisible[x][y] == UNKNOWN)
            fieldVisible[x][y] = FLAG;
        update();
    }

    ///Routine for losing the game
    ///Reveals all undiscovered mines and misplaced flags
    void revealAll(void){
        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                if ((fieldHidden[i][j] == MINE) && (fieldVisible[i][j] == UNKNOWN) )
                    fieldVisible[i][j] = MINE;
                if ((fieldVisible[i][j] == FLAG) && (fieldHidden[i][j] != MINE) )
                    fieldVisible[i][j] = MISPLACED;
            }
        }
        update();
    }

    ///Handles mouse event
    void handleEvent( SDL_Event* e){
        int x, y;

        //Get mouse position
        if( e->type == SDL_MOUSEMOTION ){
            SDL_GetMouseState( &x, &y );
            if (playField.isInside(x,y) == false){
                inPlay = false;
            }
            else {
                inPlay = true;
            }
        }

        //Handle grid events first
        playField.handleEvent(e);

        //Refresh playing grid
        update();



        if (inPlay){
            if( e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONUP ){
                sweepEnable = true;
            }
            if( e->button.button == SDL_BUTTON_RIGHT && e->type == SDL_MOUSEBUTTONUP ){
                flagEnable = true;
            }
            if( e->motion.state || e->button.state == SDL_PRESSED ){
                if (playField.tiles[playField.col][playField.row] == UNKNOWN)
                    playField.tiles[playField.col][playField.row] = PRESS;
            }
        }
    }

        void render( SDL_Rect sprite[]){
            playField.render(sprite);
        }

};

#endif // MINESWEEPER_H_INCLUDED
