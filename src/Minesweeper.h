#ifndef MINESWEEPER_H_INCLUDED
#define MINESWEEPER_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "Const.h"
#include "Grid.h"
#include "GameState.h"
#include "Button.h"

class Minesweeper : public GameState
{
    public:

    int row;
    int col;

    int mines;
    int unexploredTiles;
    int menu_bar_width;

    int **fieldVisible;
    int **fieldHidden;

    bool sweepEnable;
    bool flagEnable;
    bool pressed;
    bool inPlay;
    bool gameOver;

    LButton resetButton;
    LButton newButton;

    //LTexture tileset;
    LTexture gSpriteSheetTexture;
    LTexture ResetSpriteSheet;
    LTexture NewSpriteSheet;
    Grid playField;

    //Test Sprite Clips
    SDL_Rect gSpriteClips[ SPRITE_NUM ];
    SDL_Rect ResetSpriteClips[ BUTTON_SPRITE_NUM ];
    SDL_Rect NewSpriteClips[ BUTTON_SPRITE_NUM ];

    ///Constructor Function
    Minesweeper(){

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Initialize playing field dimensions, difficulty, and appearance
            col = GRID_WIDTH;
            row = GRID_HEIGHT;
            mines = TOTAL_MINES;

            playField.setPos(0,MENU_BAR_HEIGHT);

            menu_bar_width = GRID_WIDTH * TILE_WIDTH;

            resetButton = LButton(menu_bar_width/2 + TILE_WIDTH, MENU_BAR_HEIGHT/4, TILE_WIDTH, TILE_HEIGHT);
            newButton = LButton(menu_bar_width/2 - (TILE_WIDTH*2), MENU_BAR_HEIGHT/4, TILE_WIDTH, TILE_HEIGHT);
            

            //Initialize visible playing field
            fieldVisible = new int *[col];
            for (int i = 0; i < col; i++)
                fieldVisible[i] = new int[row];

            //Initialize hidden playing field
            fieldHidden = new int *[col];
            for (int i = 0; i < col; i++)
                fieldHidden[i] = new int[row];

            newGame();

            //Initialize and display graphical interface
            SDL_SetWindowSize(gWindow,(GRID_WIDTH * TILE_WIDTH),(GRID_HEIGHT * TILE_HEIGHT) + (TILE_HEIGHT * 2) );
            update();
        }

    }

    ///Deconstructor
    ~Minesweeper(){
        printf("Minesweeper Object Deconstructing...\n");

        for(int i = 0; i < col; ++i) {
            delete [] fieldVisible[i];
        }
        delete [] fieldVisible;

        for(int i = 0; i < col; ++i) {
            delete [] fieldHidden[i];
        }
        delete [] fieldHidden;

        //Free loaded image
        gSpriteSheetTexture.free();
        ResetSpriteSheet.free();
        NewSpriteSheet.free();

    }

    void newGame()
    {
        //Hide all visible tiles
        for (int i = 0; i < col; i++) {
            for (int j = 0; j < row; j++)
                fieldVisible[i][j] = UNKNOWN;
        }

        //Clear playfield
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

        //Initialize unexplored tile counter and event flags
        unexploredTiles = row * col;
        sweepEnable = false;
        flagEnable = false;
        inPlay = false;
        gameOver = false;

    }

    //TODO: Can we streamline the sprite sheet creationg into a function?
    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture
        if( !gSpriteSheetTexture.loadFromFile( "assets/minesweeper_tiles_32.png") )
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

        //Load sprite sheet texture
        if( !ResetSpriteSheet.loadFromFile( "assets/reset_SS.png") )
        {
            printf( "Failed to load sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < BUTTON_SS_COLUMNS; i++){
                for (int j = 0; j < BUTTON_SS_ROWS; j++){
                    ResetSpriteClips[ n ].x = SPRITE_WIDTH * i;
                    ResetSpriteClips[ n ].y = SPRITE_HEIGHT * j;
                    ResetSpriteClips[ n ].w = SPRITE_WIDTH;
                    ResetSpriteClips[ n ].h = SPRITE_HEIGHT;
                    n++;
                }
            }
        }

        //Load sprite sheet texture
        if( !NewSpriteSheet.loadFromFile( "assets/new_SS.png") )
        {
            printf( "Failed to load sprite sheet texture!\n" );
            success = false;
        }
        else
        {
            int n = 0;
            for (int i = 0; i < BUTTON_SS_COLUMNS; i++){
                for (int j = 0; j < BUTTON_SS_ROWS; j++){
                    NewSpriteClips[ n ].x = SPRITE_WIDTH * i;
                    NewSpriteClips[ n ].y = SPRITE_HEIGHT * j;
                    NewSpriteClips[ n ].w = SPRITE_WIDTH;
                    NewSpriteClips[ n ].h = SPRITE_HEIGHT;
                    n++;
                }
            }
        }
        return success;
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

        //Handle button events first
        resetButton.handleEvent(e);
        newButton.handleEvent(e);



        if (resetButton.isClicked){
            printf("RESET\n");
            newGame();
        }

        if (newButton.isClicked){
            //Something about state change flags here
            printf("CHANGE STATE\n");
            set_next_state(STATE_MENU);
        }

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

    void logic(){

        resetButton.logic();
        newButton.logic();

        if (sweepEnable){
            if ( !sweepTile(playField.col, playField.row) ){
                printf("YOU LOSE...\n");
                gameOver = true;
            }
            else{
                if ( unexploredTiles == mines){
                    printf("YOU WIN!!!\n");
                    gameOver = true;
                }
            }
            sweepEnable = false;
        }
        if (flagEnable){
            mineFlagToggle(playField.col, playField.row);
            flagEnable = false;
        }
    }

    void render(){
        playField.render(gSpriteClips, &gSpriteSheetTexture);
        resetButton.render(ResetSpriteClips, &ResetSpriteSheet);
        newButton.render(ResetSpriteClips, &NewSpriteSheet);
    }

};

#endif // MINESWEEPER_H_INCLUDED
