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

    Minesweeper(int c, int r, int m){
        col = c;
        row = r;
        mines = m;
        unexploredTiles = row * col;
        sweepEnable = false;
        flagEnable = false;

        fieldVisible = new int *[col];
        for (int i = 0; i < col; i++)
            fieldVisible[i] = new int[row];

        fieldHidden = new int *[col];
        for (int i = 0; i < col; i++)
            fieldHidden[i] = new int[row];

        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++)
                fieldVisible[i][j] = UNKNOWN;
        }

        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++)
                fieldHidden[i][j] = EMPTY;
        }

        int n = 0;
        while (n < mines) {
            int i = rand() % col;
            int j = rand() % row;
            if (fieldHidden[i][j] != MINE) {
                fieldHidden[i][j] = MINE;
                n++;
            }
        }

        for (int i = 0; i < col; i++){
            for (int j = 0; j < row; j++){
                if (fieldHidden[i][j] != MINE)
                    fieldHidden[i][j] = setClue(i,j);
            }
        }

    }

    //Returns the number of mine tiles which are adjacent to the input coordinates
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
            if ( fieldVisible[x][y] == MINE )
                alive = false;
        }
        return alive;
    }

    void mineFlagToggle(int x, int y){
        if (fieldVisible[x][y] == FLAG)
            fieldVisible[x][y] = UNKNOWN;
        else if (fieldVisible[x][y] == UNKNOWN)
            fieldVisible[x][y] = FLAG;
    }

 /*   void press(int x, int y){
        int xTemp = 0;
        int yTemp = 0;
        DirectionVector dir;

        if (fieldVisible[x][y] == UNKNOWN)
            fieldVisible[x][y] = PRESS;

        for (int d = 0; d < 9; d++){
            dir = DIRECTIONS[d];
            xTemp = x + dir.xDif;
            yTemp = y + dir.yDif;
            if ( xTemp >= 0 && xTemp < col && yTemp >= 0 && yTemp < row){
                if (fieldVisible[xTemp][yTemp] == PRESS)
                    fieldVisible[xTemp][yTemp] = UNKNOWN;
            }
        }
    }*/

    //Handles mouse event
    void handleEvent( SDL_Event* e){
        //If mouse event happened
        if( e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONUP ){
            sweepEnable = true;
            //pressed = false;
        }
        if( e->button.button == SDL_BUTTON_RIGHT && e->type == SDL_MOUSEBUTTONUP ){
            flagEnable = true;
            //press = false;
        }
        /*if( e->button.state == SDL_PRESSED){
            pressed = true;
        }
        */
    }

};

#endif // MINESWEEPER_H_INCLUDED
